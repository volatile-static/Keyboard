#include"rgb.h"
#include"leds.h"
#include"keys.h"
#include"app_timer.h"
#include"app_scheduler.h"

const uint8_t GAMMA_STEPS[64] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 10, 12, 14, 16, 18, 20, 22, 
    24, 26, 29, 32, 35, 38, 41, 44,
    47, 50, 53, 57, 61, 65, 69, 73,
    77, 81, 85, 89, 94, 99, 104, 146, 152,
    158, 164, 170, 176, 182, 188, 195, 202,
    209, 216, 223, 230, 237, 244, 251, 255
};

enum rgb_sta_t rgb_status = rgb_staRainbow;
static struct rgb_t COLOR_DAT[COLOR_DAT_SIZE];
static uint8_t rgb_blinkProgress[17];

APP_TIMER_DEF(RAINBOW_TIM);
APP_TIMER_DEF(KEYBOARD_TIM);

static void GenerateColorData(void) {
    uint8_t i, j;
    for (i = 0; i < 6; ++i)
        for (j = 0; j <= MAX_BRIGHTNESS; ++j)
            switch (i) {
                case 0:
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].green = j;
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].red = MAX_BRIGHTNESS;
                    break;
                case 1:
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].red = MAX_BRIGHTNESS - j;
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].green = MAX_BRIGHTNESS;
                    break;
                case 2:
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].blue = j;
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].green = MAX_BRIGHTNESS;
                    break;
                case 3:
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].green = MAX_BRIGHTNESS - j;
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].blue = MAX_BRIGHTNESS;
                    break;
                case 4:
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].red = j;
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].blue = MAX_BRIGHTNESS;
                    break;
                case 5:
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].blue = MAX_BRIGHTNESS - j;
                    COLOR_DAT[i*MAX_BRIGHTNESS + j].red = MAX_BRIGHTNESS;
                    break;
            }
}

void rgb_setRow(uint8_t row, uint16_t phase) {
    uint8_t i;
    for (i = 0; i < 17; ++i) {
        if (led_pos[i].row == row)
            fl3236_setRGB(i, COLOR_DAT[phase].red, COLOR_DAT[phase].green, 
                COLOR_DAT[phase].blue);
    }
    fl3236_updateReg();
}

void rainbow_timeout_handler(void *p) {
    static uint16_t cnt;
    if (rgb_status != rgb_staRainbow)
        return;
    ++cnt;
    cnt %= COLOR_DAT_SIZE;
    //NRF_LOG_INFO("%x %x %x" , rgb_val.red, rgb_val.green, rgb_val.blue);
    rgb_setRow(0, (cnt + 2)%COLOR_DAT_SIZE);
    rgb_setRow(1, (cnt + 4)%COLOR_DAT_SIZE);
    rgb_setRow(2, (cnt + 6)%COLOR_DAT_SIZE);
    rgb_setRow(3, (cnt + 8)%COLOR_DAT_SIZE);
    rgb_setRow(4, (cnt + 10)%COLOR_DAT_SIZE);
}

void keyboard_timeout_handler(void *p) {
    if (rgb_status != rgb_staKeyboard)
        return;
    for (uint_fast8_t i = 1; i < 17; ++i) {
        if (rgb_blinkProgress[i])
            --rgb_blinkProgress[i];
        fl3236_setRGB(i, 0, rgb_blinkProgress[i], 0x0c);
    }
    fl3236_updateReg();
}

void rgb_init(void) {
    ret_code_t err;
    GenerateColorData();
    err = app_timer_create(&RAINBOW_TIM, APP_TIMER_MODE_REPEATED, rainbow_timeout_handler);
    APP_ERROR_CHECK(err);
    err = app_timer_create(&KEYBOARD_TIM, APP_TIMER_MODE_REPEATED, keyboard_timeout_handler);
    APP_ERROR_CHECK(err);
    err = app_timer_start(RAINBOW_TIM, APP_TIMER_TICKS(RAINBOW_INTERVAL_MS), NULL);
    APP_ERROR_CHECK(err);
    err = app_timer_start(KEYBOARD_TIM, APP_TIMER_TICKS(KEYBOARD_INTERVAL_MS), NULL);
    APP_ERROR_CHECK(err);
}

void rgb_numLock(bool on) {
    if (on) {
        fl3236_setRGB(0, 0x06, 0x7f, 0);
    } else {
        fl3236_setRGB(0, 0x0f, 0, 0);
    }
    fl3236_updateReg();
}

void rgb_keyboardLoad(void) {
    for (uint_fast8_t i = 1; i < 17; ++i) {
        if (KeyDown[i])
            rgb_blinkProgress[i] = 0x2f;
    }
}
