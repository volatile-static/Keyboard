/*
 * key.h
 *
 *  Created on: 2021Äê5ÔÂ9ÈÕ
 *      Author: hlK
 */

#ifndef KEY_H_
#define KEY_H_

#include <stdbool.h>
#include <stdint.h>

#define KEY_ID_MASK (0x7f)
#define KEY_DOWN_MASK (0x80)

typedef struct {
	bool valid, down;
	uint8_t code;
} key_flow_t;

static const unsigned char KEY2LED[103] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 74, 75, 76,  // row0
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 77, 78, 79, 80, 81, 82, 83,  // row1
    27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 84, 85, 86, 87, 88, 89, 90,  // row2
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 91, 92, 93,  // row3
    54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 94, 95, 96, 97,  // row4
    66, 67, 68, 69, 70, 71, 72, 98, 99, 100, 101, 102, 103,  // row5
};

void key_init(void);
key_flow_t key_get(void);
void key_set(void);


#endif /* KEY_H_ */
