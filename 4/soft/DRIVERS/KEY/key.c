#include "key.h"
#include "delay.h"

void key_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =
		GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

u8 key_scan() {
	u8 k = 0;
	if (!KEY0 || !KEY1 || !KEY2 || !KEY3) {
		delay_ms(9); // 去抖动
		if (KEY0 == 0)
			++k;
		if (KEY1 == 0)
			k |= 2;
		if (KEY2 == 0)
			k |= 4;
		if (KEY3 == 0)
			k |= 8;
	}
	return k;
}
