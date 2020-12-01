#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------ */
#include "platform_config.h"
#include "usb_type.h"

/* Exported types ------------------------------------------------------------ */
/* Exported constants -------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------ */
/* Exported define ----------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
void USB_Port_Set(u8 enable);
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void Joystick_Send(uint8_t Key0, uint8_t Key1, uint8_t Key2, uint8_t Key3, uint8_t Key4, uint8_t Key5, uint8_t Key6);
void Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
