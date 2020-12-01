#ifndef DELAYH
#define DELAYH

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif
    void delay_init(void);
    void delay_us(u32 nus);
		void delay_ms(u32 nms);
#ifdef __cplusplus
}
#endif

#endif
