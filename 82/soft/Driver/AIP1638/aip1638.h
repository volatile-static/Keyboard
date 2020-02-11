#include "delay.h"
#ifdef __cplusplus
extern "C" {
#endif
		void aip1638_init(void);
	  // void aip1638_writeCmd(u8 Cmd);
	  // void aip1638_writeData(u8 Addr, u8 Data);
		void aip1638_readKey(u8 KeyPressed[24]);
		void aip1638_setLED(u8 Data[80]);
#ifdef __cplusplus
}
#endif	
