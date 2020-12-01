#include "led.h"
#include "stm32f10x.h"

void tim_init() {
		NVIC_InitTypeDef NVIC_InitStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE); 
	
		TIM_TimeBaseStructure.TIM_Period = 2333; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
		TIM_TimeBaseStructure.TIM_Prescaler = 26; //设置用来作为TIMx时钟频率除数的预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
		NVIC_Init(&NVIC_InitStructure);
	
		TIM_TimeBaseStructure.TIM_Period = 899; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
		TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置用来作为TIMx时钟频率除数的预分频值
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

		TIM_Cmd(TIM3, ENABLE);
		TIM_Cmd(TIM4, ENABLE);
}

void led_init(void) {
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
		
		tim_init();
}

void TIM3_IRQHandler(void) {
		static int cnt = 0, flag = 0;
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
				//LED2 = !LED2;
				if (cnt >= 800)
						flag = -1;
				if (cnt <= 30)
						flag = 1;
				cnt += flag;
				TIM_SetCompare3(TIM4, 800 - cnt);
				TIM_SetCompare4(TIM4, cnt);
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除 TIM3 更新中断标志
		}
}
