#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"   
#include "ps2.h" 
#include "keyboard.h"
//https://wenku.baidu.com/view/0308d6890508763230121203.html


PS2dev kbd;

int main(void) {
	delay_init();	    	 //延时函数初始化	
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	// LCD_Init();			   	//初始化LCD     
	kbd.keyboard_init();

	while (1) {
		unsigned char led;
		if (kbd.keyboard_handle(&led))
			LED1 = !LED1;

		//		LCD_ShowNum(2, 3, t, 6, 24);
		//		t++;
		//		POINT_COLOR = t;
		// delay_ms(100);
		//		if (t % 20 == 0) {
		//			LED0 = !LED0;
		//		}
	}
}
