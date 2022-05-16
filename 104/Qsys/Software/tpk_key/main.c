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

#include <io.h>
#include <stdio.h>
#include <system.h>
#include <unistd.h>
#include <altera_avalon_uart_regs.h>
#include "key.h"
#include "ps2_tx.h"

int main() {
	unsigned cpuid = __builtin_rdctl(5);
	printf("Hello from Nios II%d(Key)!\n", cpuid);
//	key_init();

	for (unsigned tim = _alt_nticks; ;) {
		for (char i = 1; i < 103; ++i) {
			unsigned sta = IORD(KEY_STREAM_BASE, i);
			if (sta & 1) {
				if (sta & 2)
					ps2_push(i - 1);
				else
					ps2_release(i - 1);
			}
		}

//		key_flow_t k = key_get();
//		if (k.valid) {
//			key_set();
//			if (k.down)
//				ps2_make(k.code);
//			else
//				ps2_break(k.code);
//		}
	}
	return 0;
}
