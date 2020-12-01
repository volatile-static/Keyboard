#include "led.h"					
void LED_Init(void)					//LED IO初始化
{
	RCC->APB2ENR|=1<<2;				//使能PORTA时钟
	RCC->APB2ENR|=1<<3;				//使能PORTB时钟
	JTAG_Set(JTAG_SWD_DISABLE);		//关闭JTAG,SWD
	GPIOA->CRH&=0XFF00FFF0;
	GPIOA->CRH|=0X00330003;			//PA8.13.14推挽输出
	GPIOA->ODR|=1<<13;				//PA13输出高
	GPIOA->ODR|=1<<14;				//PA14输出高.PA8默认低
	GPIOB->CRL&=0X00000000;
	GPIOB->CRL|=0X33333333;			//PB0~7推挽输出
	GPIOB->ODR&=0XFF00;				//PB0~7输出高
	GPIOB->ODR|=0X00FF;
}

void LED_Shine(u8 led)
{
	switch(led)
		{
				case 1:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X008E;break;
				case 2:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X008D;break;
				case 3:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X008B;break;
				case 4:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X0087;break;
				case 5:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X004E;break;
				case 6:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X004D;break;
				case 7:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X004B;break;
				case 8:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X0047;break;
				case 9:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X002E;break;
				case 10:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X002D;break;
				case 11:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X002B;break;
				case 12:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X0027;break;
				case 13:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X001E;break;
				case 14:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X001D;break;
				case 15:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X001B;break;
				case 16:GPIOB->ODR&=0XFF00,GPIOB->ODR|=0X0017;break;
				case 17:L1=0;break;
				case 18:LA=0;break;
				case 19:PA8=1;break;
		}
}

