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
	led_init();
	key_init();

	for (char flag = 0; ; ) {
//		key_flow_t k = key_get();
//		if (k.valid) {
//			key_set();
////			if (k.down)printf("keyd: 0x%x\n", k.code);
//			SET_LED(KEY2LED[k.code], k.down ? 0xffffff : 0);
//		}
		led_sched();
		unsigned tmp = IORD(PS2_PS2RX_BASE, PS2_RX_OFST);
		if (tmp & PS2_RX_VALID_MSK) {
			if ((tmp & PS2_RX_PAYLOAD_MSK) == 0xed)
				++flag;
			else if (flag) {
				--flag;
				SET_LED(SCR_LOCK, tmp & PS2_SCRLK_MSK ? 255 : 0);
				SET_LED(CAPS_LOCK, tmp & PS2_CAPLK_MSK ? 255 : 0);
				SET_LED(NUM_LOCK, tmp & PS2_NUMLK_MSK ? 255 : 0);
			}
		}
	}
	return 0;
}
