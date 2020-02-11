# include "sys.h"
# include "usart.h"
# include "delay.h"
# include "led.h"
# include "key.h"
# include "usb_lib.h"
# include "hw_config.h"
# include "usb_pwr.h" 
# include "timer.h"

int main(void)
{
	u16 key=0;
	Stm32_Clock_Init(9);			//系统时钟设置
	delay_init(72);					//延时初始化
	LED_Init();						//LED初始化
	Key_Init();						//按键初始化
	TIM3_Int_Init(1000,7199);		//10KHZ的计数频率，计数到1000为100ms
	
	delay_ms(300);
 	usb_port_set(0); 				//USB先断开
	delay_ms(300);
   	usb_port_set(1);				//USB再次连接
									
 	USB_Interrupts_Config();    	//USB配置
 	Set_USBClock();   
 	USB_Init();
	
	while(1)
	{
		u8 Buf[8]={0,0,0,0,0,0,0,0};
		u8 i=2;
		
		key=Key_Scan();				//调用按键扫描程序获得键值
		
		if(key&KX_PRES)
		{	
			L1=0;LA=1;
			LED_Shine(15);
			if(key&KZ_PRES)		//LeftAlt/Z
			{
				Buf[0]|=0X04;
				LED_Shine(14);
			}
			if(key&K1_PRES)		//ESC
			{
				Buf[i]=0X29;
				LED_Shine(1);
				i++;
			}
			if(key&K2_PRES)		//6
			{
				Buf[i]=0X23;
				LED_Shine(2);
				i++;
			}
			if(key&K3_PRES)		//7
			{
				Buf[i]=0X24;
				LED_Shine(3);
				i++;
			}
			if(key&K4_PRES)		//8
			{
				Buf[i]=0X25;
				LED_Shine(4);
				i++;
			}
			if(key&K5_PRES)		//9
			{
				Buf[i]=0X26;
				LED_Shine(13);
				i++;
			}
			if(key&KQ_PRES)		//Z
			{
				Buf[i]=0X1D;
				LED_Shine(5);
				i++;
			}
			if(key&KW_PRES)		//X
			{
				Buf[i]=0X1B;
				LED_Shine(6);
				i++;
			}
			if(key&KE_PRES)		//C
			{
				Buf[i]=0X06;
				LED_Shine(7);
				i++;
			}
			if(key&KR_PRES)		//V
			{
				Buf[i]=0X19;
				LED_Shine(8);
				i++;
			}
			if(key&KA_PRES)		//T
			{
				Buf[i]=0X17;
				LED_Shine(9);
				i++;
			}
			if(key&KS_PRES)		//G
			{
				Buf[i]=0X0A;
				LED_Shine(10);
				i++;
			}
			if(key&KD_PRES)		//B
			{
				Buf[i]=0X05;
				LED_Shine(11);
				i++;
			}
			if(key&KF_PRES)		//M
			{
				Buf[i]=0X10;
				LED_Shine(12);
				i++;
			}
			if(key&KSP_PRES)	//TAB
			{
				Buf[i]=0X2B;
				LED_Shine(16);
				i++;
			}
		}
		else if((key&KX_PRES)==0)
		{	
			L1=1;LA=0;
			if(key&KZ_PRES)		//LeftShift/Z
			{
				Buf[0]|=0X02;
				LED_Shine(14);
			}
			if(key&K1_PRES)		//1
			{
				Buf[i]=0X1E;
				LED_Shine(1);
				i++;
			}
			if(key&K2_PRES)		//2
			{
				Buf[i]=0X1F;
				LED_Shine(2);
				i++;
			}
			if(key&K3_PRES)		//3
			{
				Buf[i]=0X20;
				LED_Shine(3);
				i++;
			}
			if(key&K4_PRES)		//4
			{
				Buf[i]=0X21;
				LED_Shine(4);
				i++;
			}
			if(key&K5_PRES)		//5
			{
				Buf[i]=0X22;
				LED_Shine(13);
				i++;
			}
			if(key&KQ_PRES)		//Q
			{
				Buf[i]=0X14;
				LED_Shine(5);
				i++;
			}
			if(key&KW_PRES)		//W
			{
				Buf[i]=0X1A;
				LED_Shine(6);
				i++;
			}
			if(key&KE_PRES)		//E
			{
				Buf[i]=0X08;
				LED_Shine(7);
				i++;
			}
			if(key&KR_PRES)		//R
			{
				Buf[i]=0X15;
				LED_Shine(8);
				TIM3->CR1|=0x01;
				PA8=1;
				i++;
			}
			if(key&KA_PRES)		//A
			{
				Buf[i]=0X04;
				LED_Shine(9);
				i++;
			}
			if(key&KS_PRES)		//S
			{
				Buf[i]=0X16;
				LED_Shine(10);
				i++;
			}
			if(key&KD_PRES)		//D
			{
				Buf[i]=0X07;
				LED_Shine(11);
				i++;
			}
			if(key&KF_PRES)		//F
			{
				Buf[i]=0X09;
				LED_Shine(12);
				i++;
			}
			if(key&KSP_PRES)	//Spacebar
			{
				Buf[i]=0X2C;
				LED_Shine(16);
				i++;
			}
		}
			
		Joystick_Send(Buf[0],Buf[1],Buf[2],Buf[3],Buf[4],Buf[5],Buf[6],Buf[7]);
	}
}

