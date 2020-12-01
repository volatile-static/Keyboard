#include "key.h"
#include "delay.h"
#include "pin_def.h"
#include "stm32f10x.h"
#include <string.h>

void key_init(void) {
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | (GPIO_Pin_8 - GPIO_Pin_3);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 - GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void key_scan(u8 KeyDown[]) {
		u8 i, j;
		//memset(KeyDown, 0, 480);
		for (i = 0; i < 60; ++i)
				KeyDown[i] = 0;
		GPIOB->ODR &= 0xff05; // pb1,3,4,5,6,7=0 
		delay_us(1);
		if ((GPIOA->IDR & 0x03ff) != 0x03ff) {
				delay_ms(6);
				GPIOB->ODR |= 0x00f8;
				delay_us(1);
				for (i = 1; i < 9; ++i)
						KeyDown[i] = !PAin(i);
				
				for (j = 3; j < 8; ++j) {
						GPIOB->ODR &= 0xff05;
						GPIOB->ODR |= ~(0x01 << j) & 0xfa;
						delay_us(1);
						for (i = 0; i < 10; ++i)
								KeyDown[(j - 2)*10 + i] = !PAin(i);
				}
		}
}
