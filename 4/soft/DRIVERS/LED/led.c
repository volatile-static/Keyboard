#include "led.h"		

void led_init(void)					//LED IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9);
}

void led_cmd(u8 id, u8 on) {
	switch (id) {
		case 0:
			PBout(8) = on;
			break;
		case 1:
			PBout(9) = on;
			break;
		case 2:
			PBout(6) = on;
			break;
		case 3:
			PBout(7) = on;
			break;
		default: break;
	}
}
