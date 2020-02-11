#ifndef KEYS_H
#define KEYS_H

#include"bsp.h"
#include<stdint.h>
#include<stdbool.h>

#define KEY_CNT 19

extern bool KeyDown[KEY_CNT];
extern const uint8_t Key2HID[17];

void tca8418_init(void);
void tca8418_keyScan(void);
void tca8418_logAll(void);
void tca8418_logReg(uint8_t reg);
void tca8418_clearInterrupts(void);
void keyboard_clearKeyDown(void);
void keys_eventHandler(bsp_event_t event);

#endif
