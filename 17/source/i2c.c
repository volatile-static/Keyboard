#include "i2c.h"
#include "boards.h"
#include "nrfx_twim.h"

static const nrfx_twim_t m_twi = NRFX_TWIM_INSTANCE(0);
volatile static bool m_flag;
static uint8_t m_value;

void i2c_writeBuffer(uint8_t addr, uint8_t *buf, uint8_t len) {
    nrfx_twim_xfer_desc_t fer = NRFX_TWIM_XFER_DESC_TX(addr, buf, len);
    m_flag = true;
    APP_ERROR_CHECK(nrfx_twim_xfer(&m_twi, &fer, NULL));
    while (m_flag) {
    }
}

void i2c_writeByte(uint8_t addr, uint8_t reg, uint8_t dat) {
    uint8_t buf[2] = { reg, dat };    
    nrfx_twim_xfer_desc_t fer = NRFX_TWIM_XFER_DESC_TX(addr, buf, 2);
    I2C_WAIT(m_twi);
    APP_ERROR_CHECK(nrfx_twim_xfer(&m_twi, &fer, NULL));
}

uint8_t i2c_readByte(uint8_t addr, uint8_t reg) {
    nrfx_twim_xfer_desc_t fer = NRFX_TWIM_XFER_DESC_TXRX(addr, &reg, 1, &m_value, 1);
    m_flag = true;
    I2C_WAIT(m_twi);
    APP_ERROR_CHECK(nrfx_twim_xfer(&m_twi, &fer, NULL));
    while (m_flag) {
    }    
    return m_value;
}

void i2c_handler(nrfx_twim_evt_t const *p_event, void *p_context) {
    if (p_event->type == NRFX_TWIM_EVT_DONE && 
        p_event->xfer_desc.type == NRFX_TWIM_XFER_TXRX)
        m_flag = false;  
}

void i2c_init(void) {
    nrfx_twim_config_t const config = {
        .scl = ARDUINO_SCL_PIN,
        .sda = ARDUINO_SDA_PIN,
        .frequency = (nrf_twim_frequency_t)NRFX_TWIM_DEFAULT_CONFIG_FREQUENCY,             
        .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,                   
        .hold_bus_uninit    = NRFX_TWIM_DEFAULT_CONFIG_HOLD_BUS_UNINIT,   
    };
    APP_ERROR_CHECK(nrfx_twim_init(&m_twi, &config, i2c_handler, NULL));
    nrfx_twim_enable(&m_twi);
}
