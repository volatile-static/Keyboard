#include "usb.h"
#include "delay.h"
#include "usb_lib.h"
#include "usb_pwr.h"

void usb_init(void) {
    Set_System();
    delay_ms(200);
    USB_Port_Set(0); 	//USBÏÈ¶Ï¿ª
    delay_ms(100);
    USB_Port_Set(1); 
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
}

u8 usb_isConfigured(void) {
    return bDeviceState == CONFIGURED;
}

void usb_send(u8 Keys[7]) {
	u8 Buffer[8] = {Keys[0], 0, Keys[1], Keys[2], Keys[3], Keys[4], Keys[5], Keys[6]}; 
	/* Copy key info in ENDP1 Tx Packet Memory Area */
	USB_SIL_Write(EP1_IN, Buffer, 8);
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP1);
}
