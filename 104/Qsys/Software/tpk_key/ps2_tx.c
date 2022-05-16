/*
+	q	q * ps2.c
 *
 *  Created on: 2021Äê5ÔÂ10ÈÕ
 *      Author: hlK
 */

#include "ps2_tx.h"

#include <stdio.h>
#include <system.h>
#include <unistd.h>

#define BYTE_DELAY (2600)
#define TYPEMATIC_MS (50)

static const unsigned char ps2_scan_code_table[103][2] = {
	{0x76,0}, {0x05,0}, {0x06,0}, {0x04,0}, {0x0c,0}, {0x03,0}, {0x0b,0}, {0x83,0}, {0x0a,0}, {0x01,0}, {0x09,0}, {0x78,0}, {0x07,0}, {0,0}, {0x7e,0}, {0,0},
	{0x0e,0}, {0x16,0}, {0x1e,0}, {0x26,0}, {0x25,0}, {0x2e,0}, {0x36,0}, {0x3d,0}, {0x3e,0}, {0x46,0}, {0x45,0}, {0x4e,0}, {0x55,0}, {0x66,0}, {0x70,1}, {0x6c,1}, {0x7d,1}, {0x77,0}, {0x4a,1}, {0x7c,0}, {0x7b,0},
	{0x0d,0}, {0x15,0}, {0x1d,0}, {0x24,0}, {0x2d,0}, {0x2c,0}, {0x35,0}, {0x3c,0}, {0x43,0}, {0x44,0}, {0x4d,0}, {0x54,0}, {0x5b,0}, {0x5d,0}, {0x71,1}, {0x69,1}, {0x7a,1}, {0x6c,0}, {0x75,0}, {0x7d,0}, {0x79,0},
	{0x58,0}, {0x1c,0}, {0x1b,0}, {0x23,0}, {0x2b,0}, {0x34,0}, {0x33,0}, {0x3b,0}, {0x42,0}, {0x4b,0}, {0x4c,0}, {0x52,0}, {0x5a,0}, {0x6b,0}, {0x73,0}, {0x74,0},
	{0x12,0}, {0x1a,0}, {0x22,0}, {0x21,0}, {0x2a,0}, {0x32,0}, {0x31,0}, {0x3a,0}, {0x41,0}, {0x49,0}, {0x4a,0}, {0x59,0}, {0x75,1}, {0x69,0}, {0x72,0}, {0x7a,0},
	{0x14,0}, {0x1f,1}, {0x11,0}, {0x29,0}, {0x11,1}, {0x2f,1}, {0x14,1}, {0x6b,1}, {0x72,1}, {0x74,1}, {0x70,0}, {0x71,0}, {0x5a,1}
};
static alt_alarm timer_delay, timer_repeat;
static unsigned char typematic_kid;

void ps2_make(unsigned char kid) {
	usleep(BYTE_DELAY);
	if (ps2_scan_code_table[kid][1]) {
		IOWR(PS2_PS2TX_BASE, PS2_TX_OFST, 0xe0);
		usleep(BYTE_DELAY);
	}
	IOWR(PS2_PS2TX_BASE, PS2_TX_OFST, ps2_scan_code_table[kid][0]);
//	printf("id=%x make=%x\n", kid, ps2_scan_code_table[kid][0]);
}

void ps2_break(unsigned char kid) {
	usleep(BYTE_DELAY);
	if (ps2_scan_code_table[kid][1]) {
		IOWR(PS2_PS2TX_BASE, PS2_TX_OFST, 0xe0);
		usleep(BYTE_DELAY);
	}
	IOWR(PS2_PS2TX_BASE, PS2_TX_OFST, 0xf0);
	usleep(BYTE_DELAY);
	IOWR(PS2_PS2TX_BASE, PS2_TX_OFST, ps2_scan_code_table[kid][0]);
//	printf("id=%x break=%x\n", kid, ps2_scan_code_table[kid][0]);
}

void ps2_release(unsigned char kid) {
	ps2_break(kid);
	alt_alarm_stop(&timer_delay);
	alt_alarm_stop(&timer_repeat);
}

alt_u32 repeat_callback(void* context) {
	ps2_make(typematic_kid);
	return TYPEMATIC_MS;
}

alt_u32 delay_callback(void* context) {
	alt_alarm_stop(&timer_delay);
    alt_alarm_start(&timer_repeat, TYPEMATIC_MS, repeat_callback, NULL);
	return 0;
}

void ps2_push(unsigned char kid) {
	ps2_make(kid);
	typematic_kid = kid;
	alt_alarm_stop(&timer_delay);
	alt_alarm_stop(&timer_repeat);
    alt_alarm_start(&timer_delay, 600, delay_callback, NULL);
}

void ps2_init(void) {
}
