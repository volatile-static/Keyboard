#include"leds.h"
#include"i2c.h"
#include"bsp.h"
#include"nrf_log.h"

#define FL3236_ADDR0 0x3C
#define FL3236_ADDR1 0x3F

struct {
    uint8_t addr, chR, chG, chB;
} const RGBLEDs[17] = {
    { FL3236_ADDR0, 31, 30, 24 },
    { FL3236_ADDR0, 35, 34, 33 },
    { FL3236_ADDR0, 5, 4, 2 },
    { FL3236_ADDR0, 13, 12, 6 },
    { FL3236_ADDR1, 1, 2, 3 },
    { FL3236_ADDR1, 23, 22, 27 },
    { FL3236_ADDR1, 24, 26, 25 },
    { FL3236_ADDR0, 20, 21, 22 },
    { FL3236_ADDR1, 19, 20, 21 },
    { FL3236_ADDR1, 12, 13, 14 },
    { FL3236_ADDR1, 4, 7, 8 },
    { FL3236_ADDR1, 31, 32, 33 },
    { FL3236_ADDR1, 16, 17, 18 },
    { FL3236_ADDR1, 10, 11, 15 },
    { FL3236_ADDR1, 5, 6, 9 },
    { FL3236_ADDR1, 28, 29, 30 },
    { FL3236_ADDR1, 34, 35, 36 }
};

void fl3236_updateReg(void) {
    i2c_writeByte(FL3236_ADDR0, 0x25, 0);
    i2c_writeByte(FL3236_ADDR1, 0x25, 0);
}

void fl3236_setFreq(bool Hz22k) {
    i2c_writeByte(FL3236_ADDR0, 0x4B, Hz22k);
    i2c_writeByte(FL3236_ADDR1, 0x4B, Hz22k);
}

void fl3236_setAll(uint8_t val) {
    uint8_t i;
    for (i = 1; i <= 0x24; ++i) {
        i2c_writeByte(FL3236_ADDR1, i, val);
        i2c_writeByte(FL3236_ADDR0, i, val);
    }
}

void fl3236_setCurr(uint8_t mode) {
    uint8_t i;
    if (mode > 3)
        mode = 3;
    for (i = 0x26; i <= 0x49; ++i) {
        i2c_writeByte(FL3236_ADDR0, i, mode << 1);
        i2c_writeByte(FL3236_ADDR1, i, mode << 1);
    }
}

void fl3236_enable(bool en) {
    uint8_t i;
    for (i = 0x26; i <= 0x49; ++i) {
        i2c_writeByte(FL3236_ADDR0, i, en);
        i2c_writeByte(FL3236_ADDR1, i, en);
    }
    i2c_writeByte(FL3236_ADDR0, 0, en);
    i2c_writeByte(FL3236_ADDR1, 0, en);
}

void fl3236_init(void) {
    fl3236_enable(false);
    fl3236_setAll(0x00);
    fl3236_setCurr(3);
    fl3236_setFreq(true);
    fl3236_updateReg();
    fl3236_enable(true);
    NRF_LOG_INFO("3236ready");
}

void fl3236_setRGB(uint8_t id, uint8_t r, uint8_t g, uint8_t b) {
    i2c_writeByte(RGBLEDs[id].addr, RGBLEDs[id].chR, r);
    i2c_writeByte(RGBLEDs[id].addr, RGBLEDs[id].chG, g);
    i2c_writeByte(RGBLEDs[id].addr, RGBLEDs[id].chB, b);
}
