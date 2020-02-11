# include "key.h"
# include "delay.h"

void Key_Init(void)						//按键初始化
{
	RCC->APB2ENR|=1<<2;       	        //使能PORTA时钟 
	GPIOA->CRL&=0X00000000;  
	GPIOA->CRL|=0X88883333;     	    //PA3~PA0推挽输出1，PA7~4上拉输入
	GPIOA->ODR&=0XFF00;
	GPIOA->ODR|=0X00FF;
}

u16 Key_Scan(void)						//按键扫描法
{	
	u16 key=0;							//定义返回变量
	
	GPIOA->ODR&=0XFFF0;					//PA3~PA0输出0000
	GPIOA->ODR|=0X0000;
	delay_us(1);						//给反应时间
	if((GPIOA->IDR & 0X00F0)!=0X00F0)
	{
		delay_ms(10);					//去抖动 
		
		GPIOA->ODR&=0XFFF0;				//PA3~PA0输出1110
		GPIOA->ODR|=0X000E;
		delay_us(1);					//给反应时间	
		if(h1==0)
		{	key|=0X0001;	}	//1	
		if(h2==0)
		{	key|=0X0010;	}	//Q
		if(h3==0)
		{	key|=0X0100;	}	//A
		if(h4==0)
		{	key|=0X1000;	}	//5
			
		GPIOA->ODR&=0XFFF0;				//PA3~PA0输出1101
		GPIOA->ODR|=0X000D;
		delay_us(1);					//给反应时间
		if(h1==0)
		{	key|=0X0002;	}	//2	
		if(h2==0)
		{	key|=0X0020;	}	//W
		if(h3==0)
		{	key|=0X0200;	}	//S
		if(h4==0)
		{	key|=0X2000;	}	//Z
		
		GPIOA->ODR&=0XFFF0;				//PA3~PA0输出1011
		GPIOA->ODR|=0X000B;
		delay_us(1);					//给反应时间
		if(h1==0)
		{	key|=0X0004;	}	//3	
		if(h2==0)
		{	key|=0X0040;	}	//E
		if(h3==0)
		{	key|=0X0400;	}	//D
		if(h4==0)
		{	key|=0X4000;	}	//X
		
		GPIOA->ODR&=0XFFF0;				//PA3~PA0输出0111
		GPIOA->ODR|=0X0007;
		delay_us(1);					//给反应时间
		if(h1==0)
		{	key|=0X0008;	}	//4	
		if(h2==0)
		{	key|=0X0080;	}	//R
		if(h3==0)
		{	key|=0X0800;	}	//F
		if(h4==0)
		{	key|=0X8000;	}	//SP						
	}	
	return key;							//返回键值
}


