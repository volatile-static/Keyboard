#include "usb.h"
#include "delay.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "hw_config.h"

void usb_init(void) {
	Set_System();
	delay_ms(600);
 	USB_Port_Set(0); 	//USBÏÈ¶Ï¿ª
	delay_ms(300);
  USB_Port_Set(1); 
  USB_Interrupts_Config();
  Set_USBClock();
	USB_Init();
}

unsigned char usb_IsConfigured(void) {
	return bDeviceState == CONFIGURED;
}

void usb_Send(unsigned char *Keys) {
	Joystick_Send(Keys[0], Keys[1], Keys[2], Keys[3], Keys[4], Keys[5], Keys[6]);
	//Joystick_Send(0, 0, 0, 0, 0, 0, 0);
}
