/*
 * key.c
 *
 *  Created on: 2021年5月9日
 *      Author: hlK
 */

#include "key.h"
#include <stdint.h>
#include <system.h>
#include <altera_avalon_uart_regs.h>
#include <priv/alt_legacy_irq.h>

//static bool key_status[103];
key_flow_t key_flow;

void key_isr(void* isr_context) {
	unsigned payload = IORD(KEY_STREAM_BASE, 0);
	key_flow.valid = true;
	key_flow.down = payload & KEY_DOWN_MASK;
	key_flow.code = payload & KEY_ID_MASK;
}

void key_init(void) {
	int err = alt_irq_register(KEY_STREAM_IRQ, NULL, key_isr);  //注册中断
	printf("kirq:%d\n", err);
}
