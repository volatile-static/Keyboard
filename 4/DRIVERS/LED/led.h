#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 PBout(8);
#define LED1 PBout(9);
#define LED2 PBout(6);
#define LED3 PBout(7);
	 
#ifdef __cplusplus
extern "C" {
#endif
	
void led_init(void);
void led_cmd(u8 id, u8 on);
		
#ifdef __cplusplus
}
#endif

#endif 
