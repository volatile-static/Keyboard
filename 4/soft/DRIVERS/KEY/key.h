#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
#define KEY0 !PAin(4)
#define KEY1 !PAin(5)
#define KEY2 !PAin(6)
#define KEY3 !PAin(7)
	 
#ifdef __cplusplus
extern "C" {
#endif
	
void key_init(void);
u8 key_scan(void);	

#ifdef __cplusplus
}
#endif

#endif

