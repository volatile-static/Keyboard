#ifndef LEDS_H
#define LEDS_H

#include<stdint.h>

struct {
    uint8_t row, col;
} static const led_pos[17] = {
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 1, 0 },
    { 1, 1 },
    { 1, 2 },
    { 2, 0 },
    { 2, 1 },
    { 2, 2 },
    { 1, 3 },
    { 3, 0 },
    { 3, 1 },
    { 3, 2 },
    { 3, 3 },
    { 4, 1 },
    { 4, 2 }
};

void fl3236_init(void);
void fl3236_updateReg(void);
void fl3236_setRGB(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
    
#endif
