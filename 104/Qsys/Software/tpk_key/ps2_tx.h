/*
 * ps2.h
 *
 *  Created on: 2021Äê5ÔÂ10ÈÕ
 *      Author: hlK
 */

#ifndef PS2_TX_H_
#define PS2_TX_H_

#include <io.h>
#include <sys/alt_alarm.h>

#define PS2_TX_OFST (0)

void ps2_push(unsigned char kid);
void ps2_release(unsigned char kid);

#endif /* PS2_TX_H_ */
