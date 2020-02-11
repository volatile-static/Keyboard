#include "usb.h"
#include "key.h"
#include "led.h"
#include "aip1638.h"
#include "keyboard.h"
		
u8 LEDdat[80];
		
void keyboard_init() {	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		delay_init();
		aip1638_init();
		led_init();
		key_init();
		usb_init();
	
		while (!usb_isConfigured()) {
				delay_ms(30);
				LED2 = !LED2;
		}
}

void TKeys::Send() {
	u8 cnt = 0, ScanDat[60], Buffer[7] = {0, 0, 0, 0, 0, 0, 0};
	key_scan(ScanDat);
	for (int i(1); i < 60; ++i)
		if (ScanDat[i])
			Buffer[++(cnt %= 6)] = key2usb[i];
	aip1638_readKey(ScanDat);
	for (int i(0); i < 24; ++i)
		if (ScanDat[i]) {
			switch (i) {
				case 1: case 3: case 4: case 5: 
				case 11: case 17: case 20:
					Buffer[0] |= key2usb[i + 60];
					break;
				case 7: case 10:
					break;
				default:
					Buffer[++(cnt %= 6)] = key2usb[i + 60];
				}
		}
	if (ScanDat[6]) {
		LED_Caps = !LED_Caps;
		
//		static int p;
//		LEDdat[p] = 0;
//		LEDdat[++p] = 1;
//		static TLeds Led;
//		Led.Update();
	}
	LED2 = Buffer[0] > 0;
	usb_send(Buffer);
}

void TLeds::Random() {
		static int p;
		LEDdat[p %= 80] = 0;
		LEDdat[++p] = 1;
		Update();
}

void TLeds::SetAll(bool On) {
		LED_Caps = LED_Scroll = LED_Num = !(LED2 = On);
		for (int i(0); i < 80; ++i)
		    LEDdat[i] = On;
		Update();
}

void TLeds::FeedBack() {
	
	
}

void TLeds::Update() {
		aip1638_setLED(LEDdat);
}
