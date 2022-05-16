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

void key_init(void);
key_flow_t key_get(void);
void key_set(void);

#endif /* KEY_H_ */
