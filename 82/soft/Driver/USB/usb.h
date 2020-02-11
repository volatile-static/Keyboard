#ifdef __cplusplus
extern "C" {
#endif
	
	void usb_init(void);
	unsigned char usb_isConfigured(void);
	void usb_send(unsigned char *Keys);
	
#ifdef __cplusplus
}
#endif
