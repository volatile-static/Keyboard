# include "timer.h"
# include "led.h"
//定时器3中断服务程序
void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001) PA8=!PA8;	//溢出中断
	TIM3->SR&=~(1<<0);				//清除中断标志位
	TIM3->CR1&= ~(0x01);			//禁止定时器3
}
//通用定时器3中断初始化
//时钟选择为APB1的2倍，而APB1为36M
//arr:自动重装值
//psc:时钟预分频数
void TIM3_Int_Init (u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;				//TIM3时钟使能
	TIM3->ARR=arr;					//设定计数器自动重装值
	TIM3->PSC=psc;					//预分频器7200，得到10KHZ的计数时钟
	TIM3->DIER|=1<<0;				//允许更新中断
	MY_NVIC_Init(1,3,TIM3_IRQn,2);	//抢占1，子优先级3，组2
}
