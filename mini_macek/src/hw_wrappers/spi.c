#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_spi.h"
#include "ti_msp_dl_config.h"
#include "spi.h"

#define READ_DATA_VALUE 0x00
#define IS_BUSY_WAIT_COUNTER 32000  // 1ms Assuming clock freq 32MHz
#define WAIT_FOR_DATA_TCK 320000    // 10ms Assuming clock freq 32MHz
#define FLUSH_FIFO_SIZE 3           // We only need to flush 3 first elements to store incoming data

static DL_SPI_Config spi_amp_config = {
    .mode        = DL_SPI_MODE_CONTROLLER,
    .frameFormat = DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA0,
    .parity      = DL_SPI_PARITY_NONE,
    .dataSize    = DL_SPI_DATA_SIZE_8,
    .bitOrder    = DL_SPI_BIT_ORDER_MSB_FIRST,
};


/**
 * @brief Reconfigure frame format of the SPI used to communicate with amplifier
 * Due to the shitty transmition protocol, we need to read register data on 
 * first edge, but read data on the second edge
 * 
 * @param format 
 */
static void _spi_amp_reconfigure(DL_SPI_FRAME_FORMAT format) {
    DL_SPI_disable(SPI_AMP_INST);
    
    spi_amp_config.frameFormat = format;

    DL_SPI_init(SPI_AMP_INST, &spi_amp_config);

    DL_SPI_enable(SPI_AMP_INST);
}

/**
 * @brief Wait for the amplifier spi release. This just simply wait until the
 * SPI is no logner busy, or if a wait counter exceeds IS_BUSY_WAIT_COUNTER
 * 
 * @return true 
 * @return false 
 */
static bool _wait_for_spi_amp_release(void) {
    uint32_t wait_counter = 0;
    while (DL_SPI_isBusy(SPI_AMP_INST)) {
        if (wait_counter >= IS_BUSY_WAIT_COUNTER) {
            return false;
        }

        wait_counter += 1;
    }

    return true;
}

/**
 * @brief Flus rx fifo of the spi used to communicate with amplifier
 * there is two ways to flusht rx fifo: disable->enable SPI or read buffer
 */
static void _spi_amp_flush_rx_fifo(void) {
    uint8_t buffer;
    while (DL_SPI_receiveDataCheck8(SPI_AMP_INST, &buffer))
        ;
}

bool SPI_amp_set_register(uint8_t address, uint8_t value) {
    if (spi_amp_config.frameFormat == DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1) {
        _spi_amp_reconfigure(DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA0);
    } 

    DL_GPIO_clearPins(GPIO_CS_PORT, GPIO_CS_CS_PIN);

    DL_SPI_transmitData8(SPI_AMP_INST, address);
    DL_SPI_transmitData8(SPI_AMP_INST, value);

    bool res = _wait_for_spi_amp_release();

    DL_GPIO_setPins(GPIO_CS_PORT, GPIO_CS_CS_PIN);

    return res;
}


bool SPI_amp_read_register(uint8_t address, uint8_t *out_val) {
    _spi_amp_flush_rx_fifo();

    if (SPI_amp_set_register(address, READ_DATA_VALUE) == false) {
        return false;
    }

    // read first received bytes (usually 0xFF)
    if (DL_SPI_receiveDataCheck8(SPI_AMP_INST, out_val) == false) {
        return false;
    }

    // read actual value
    if (DL_SPI_receiveDataCheck8(SPI_AMP_INST, out_val) == false) {
        return false;
    }

    return true;
}

/**
 * @brief Wait for data ready signal. Yes, this shitty protocol uses the POCI line to 
 * to indicate the readiness of data. So we need to monitor the state of the POCI line to know 
 * know when data is ready to be read. Of course SPI does not have this feature.
 * so here we reconfigure the GPIO pin used for POCI to just be a casual input.
 * This function is also shitty as an amplifier communication protocol, because of the
 * GPIO pin polling, but it is what it is. Just, if it works, don't touch it.
 * @return true 
 * @return false 
 */
static bool _wait_for_data_rdy_signal(void) {
    // reconfigure SPI POCI pin to gpio input
    // Amplifier sends a signal on POCI line that data is ready, so we need
    // to observe POCI, here we use polling but it can be also done on inetrrupts
    DL_GPIO_initDigitalInputFeatures(GPIO_SPI_AMP_IOMUX_POCI, 
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE
    );

    // Ugly af and sketchy, but it works, can be change to 
    // interrupt on falling edge in the future

    // By default POCI line is low in idle, so we are waiting for the 
    // high state on POCI line, that indicates data ready.
    uint32_t wait_counter = 0;
    while (DL_GPIO_readPins(GPIO_SPI_AMP_POCI_PORT, GPIO_SPI_AMP_POCI_PIN) == false) {
        if (wait_counter > WAIT_FOR_DATA_TCK) {
            return false;
        }

        wait_counter += 1;
    }

    // Now we are waiting for the low state, to start reading data.
    wait_counter = 0;
    uint32_t countinous_low_counter = 0;
    while (countinous_low_counter < 5) {
        if (DL_GPIO_readPins(GPIO_SPI_AMP_POCI_PORT, GPIO_SPI_AMP_POCI_PIN) == false) {
            countinous_low_counter += 1;
        } else {
            countinous_low_counter = 0;
        }

        if (wait_counter > WAIT_FOR_DATA_TCK) {
            return false;
        }

        wait_counter += 1;
    }

    // Reconfigure GPIO to the SPI POCI 
    DL_GPIO_initPeripheralInputFunction(
        GPIO_SPI_AMP_IOMUX_POCI, GPIO_SPI_AMP_IOMUX_POCI_FUNC
    );

    return true;
}

bool SPI_amp_read_data(uint8_t data_buffer[3]) {
    _spi_amp_flush_rx_fifo();
    
    if (spi_amp_config.frameFormat == DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA0) {
        _spi_amp_reconfigure(DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1);
    }

    if (_wait_for_data_rdy_signal() == false) {
        return false;
    }

    // Receiving is on the HIGH CS, so we do not need to change the CS pin
    // Send 3 dummy messages, to run clocks 3 times and receive 3 data bytes.
    DL_SPI_transmitData8(SPI_AMP_INST, 0x00);
    DL_SPI_transmitData8(SPI_AMP_INST, 0x00);
    DL_SPI_transmitData8(SPI_AMP_INST, 0x00);
    
    if (_wait_for_spi_amp_release() == false) {
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        if (DL_SPI_receiveDataCheck8(SPI_AMP_INST, &data_buffer[i]) == false) {
            return false;
        }
    }

    return true;
}
