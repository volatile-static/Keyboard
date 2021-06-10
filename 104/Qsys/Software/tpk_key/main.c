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
//#include <unistd.h>
#include "key.h"
#include "ps2_tx.h"
#include <altera_avalon_uart_regs.h>

int main() {
	key_init();
	unsigned cpuid = __builtin_rdctl(5);
	printf("Hello from Nios II%d(Key)!\n", cpuid);

	while (1) {
		if (key_flow.valid) {
			printf("%d\n", key_flow.down);
			key_flow.valid = false;
			if (key_flow.down)
				ps2_make(key_flow.code);
			else
				ps2_break(key_flow.code);
		}

		IORD(KEY_STREAM_BASE, 0);

//		unsigned tmp;
//		tmp = IORD(PS2_BASE, PS2_RX_OFST);
//		if (tmp & PS2_RX_VALID_MSK)
//			printf("%x\n", tmp & PS2_RX_PAYLOAD_MSK);
	}
	return 0;
}
