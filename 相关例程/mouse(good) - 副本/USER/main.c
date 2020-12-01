#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

int main(void) {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	LED_Init();		  		
	LED0=0;					//先点亮红灯
  USB_Init();

  while (1)
  {
    
    if (bDeviceState == CONFIGURED)
    {
				LED0=0;
       // Joystick_Send(0);
    
    } 
			else LED0 = 1;
  }
}
