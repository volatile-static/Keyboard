#ifdef __cplusplus
extern "C" {
#endif
		void key_init(void);
		void key_scan(unsigned char*);
	
		const unsigned char key2usb[] = { 0,
				0x3a, /* F1 */
				0x3b, /* F2 */
				0x3c, /* F3 */
				0x3d, /* F4 */
				0x3e, /* F5 */
				0x3f, /* F6 */
				0x43, /* F10 */
				0x44, /* F11 */
				0,
			
				0x35, /* ~` */
				0x1e, /* 1 */
				0x1f, /* 2 */
				0x20, /* 3 */
				0x21, /* 4 */
				0x40, /* F7 */
				0x41, /* F8 */
				0x42, /* F9 */
				0x2e, /* += */
				0x31, /* |\ */
			
				0x14, /* Q */
				0x1a, /* W */
				0x08, /* E */
				0x22, /* 5 */
				0x23, /* 6 */
				0x24, /* 7 */
				0x25, /* 8 */
				0x26, /* 9 */
				0x27, /* 0 */
				0x2d, /* -_ */
				
				0x15, /* R */
				0x17, /* T */
				0x1c, /* Y */
				0x18, /* U */
				0x0c, /* I */
				0x12, /* O */
				0x13, /* P */
				0x2f, /* {[ */
				0x30, /* }] */
				0x34, /* "' */
				
				0x04, /* A */
				0x16, /* S */
				0x07, /* D */
				0x09, /* F */
				0x0a, /* G */
				0x0b, /* H */
				0x0d, /* J */
				0x0e, /* K */
				0x0f, /* L */
				0x33, /* :; */
				
				0x1d, /* Z */
				0x1b, /* X */
				0x06, /* C */
				0x19, /* V */
				0x05, /* B */
				0x11, /* N */
				0x10, /* M */
				0x36, /* <, */
				0x37, /* >. */
				0x38, /* ?/ */
/////////////////////////////////////////				
				0x2b, /* Tab */
				0x02, /* LShift */
				0x29, /* ESC */
				
				0x04, /* LAlt */
				0x08, /* GUI */
				0x01, /* LCtrl */
				
				0x39, /* CapsLock */
				0x02, /* LFn */
				0x2c, /* Space */
				
				0x65, /* Menu */
				0x03, /* RFn */
				0x40, /* RAlt */
				
				0x2a, /* Back */
				0x28, /* Enter */
				0x45, /* F12 */
				
				0x4d, /* End */
				0x4c, /* Delete */
				0x10, /* RCtrl */
				
				0x52, /* Up */
				0x4a, /* Home */
				0x20, /* RShift */
				
				0x51, /* Down */
				0x50, /* Left */
				0x4f, /* Right */
		};
#ifdef __cplusplus
}
#endif	
