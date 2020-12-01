#ifndef RGB_H
#define RGB_H

#include<stdint.h>
#include<stdbool.h>

#define MAX_BRIGHTNESS 0x11
#define COLOR_DAT_SIZE MAX_BRIGHTNESS*6u
#define RAINBOW_INTERVAL_MS 70
#define KEYBOARD_INTERVAL_MS 7

struct rgb_t {
    uint8_t red, green, blue;
};

enum rgb_sta_t {
    rgb_staRainbow, rgb_staKeyboard, rgb_staOff
};

extern enum rgb_sta_t rgb_status;

void rgb_init(void);
void rgb_setRow(uint8_t row, uint16_t phase);
void rgb_setInterval(uint8_t ch, uint16_t ms);
void rgb_numLock(bool on);
void rgb_keyboardLoad(void);

#endif
