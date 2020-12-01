#ifdef __cplusplus
extern "C" {
#endif
	
	void usb_init(void);
	unsigned char usb_IsConfigured(void);
	void usb_Send(unsigned char *Keys);
	
#ifdef __cplusplus
}
#endif
