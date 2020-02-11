#include "sys.h"
#include "delay.h"
#include "usb.h"
#include "key.h"
#include "led.h"
int main(void) {	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init();
	led_init();
	key_init();
  usb_init();
	
	for (u8 Ctrl(0), Keys[7] = {0, 0, 0, 0, 0, 0, 0};; usb_Send(Keys)) {
		if (KEY0) {
			delay_ms(9);
			if (KEY0)
				Ctrl = !Ctrl;
		}
		led_cmd(0, Ctrl);
		led_cmd(1, KEY1);
		led_cmd(2, KEY2);
		led_cmd(3, KEY3); 
	  Keys[0] = Ctrl;
		Keys[3] = 4 * KEY1;
		Keys[4] = 6 * KEY2;
		Keys[5] = 0x19 * KEY3;
	}
}
