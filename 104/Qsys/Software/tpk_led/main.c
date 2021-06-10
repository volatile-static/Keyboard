/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "led.h"
#include "key.h"
#include "ps2_rx.h"

int main() {
	unsigned cpuid = __builtin_rdctl(5);
	printf("Hello from Nios II %x£¨LED£©!\n", cpuid);
//	led_init();
//	key_init();

	SET_LED(CAPS_LOCK, 22);
	SET_LED(NUM_LOCK, 233);
	SET_LED(SCR_LOCK, 0);
	for (char i = 0; i < 107; ++i)
		SET_LED(i, 0xffffffffu);

	for (char flag = 0; ;) {
		IORD(KEY_STREAM_BASE, 0);
//		led_sched();
//		unsigned tmp;
//		tmp = IORD(PS2RX_BASE, PS2_RX_OFST);
//		if (tmp & PS2_RX_VALID_MSK) {
//			printf("%x\n", tmp);
//			if (flag) {
//				flag = 0;
//				SET_LED(SCR_LOCK, tmp & PS2_SCRLK_MSK ? 255 : 0);
//				SET_LED(CAPS_LOCK, tmp & PS2_CAPLK_MSK ? 255 : 0);
//				SET_LED(NUM_LOCK, tmp & PS2_NUMLK_MSK ? 255 : 0);
//			} else if ((tmp & PS2_RX_PAYLOAD_MSK) == 0xed)
//				flag = 1;
//		}
	}
	return 0;
}
