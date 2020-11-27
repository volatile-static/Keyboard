#ifndef __PS2_H
#define __PS2_H	 
	   
#include <stdbool.h>
#include "sys.h"

#define DELAY1 10
#define DELAY2 30

//物理接口定义
//PS2输入 		  				    
#define PS2_SCL PAin(15)			//PA15
#define PS2_SDA PCin(5)				//PC5
//PS2输出
#define PS2_SCL_OUT PAout(15)		//PA15
#define PS2_SDA_OUT PCout(5)		//PC5

//设置PS2_SCL输入输出状态（开漏、浮空）	  
// #define PS2_SET_SCL_IN()  (GPIOA->CRH = GPIOA->CRH & 0X0FFFFFFF | 0X40000000)
// #define PS2_SET_SCL_OUT() (GPIOA->CRH = GPIOA->CRH & 0X0FFFFFFF | 0X70000000)	  
//设置PS2_SDA输入输出状态（开漏、浮空）	 		  
// #define PS2_SET_SDA_IN()  (GPIOC->CRL = GPIOC->CRL & 0XFF0FFFFF | 0X00400000)
// #define PS2_SET_SDA_OUT() (GPIOC->CRL = GPIOC->CRL & 0XFF0FFFFF | 0X00700000) 

void ps2_wait_for_clock(void);
bool ps2_send_byte(uint8_t byte);
uint16_t ps2_recv_byte(void);
#ifdef __cplusplus
extern "C" {
void ps2_init(void);
}
#endif
#endif
