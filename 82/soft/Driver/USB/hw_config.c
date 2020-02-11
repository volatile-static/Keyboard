#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "delay.h"
#include "pin_def.h"
/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;
/* Private function prototypes ----------------------------------------------- */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);
/* Private functions --------------------------------------------------------- */

void USB_Port_Set(u8 enable) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PORTA时钟
	if (enable)
		_SetCNTR(_GetCNTR()&(~(1 << 1))); // 退出断电模式
	else {
		_SetCNTR(_GetCNTR() | (1 << 1)); // 断电模式
		GPIOA->CRH &= 0XFFF00FFF;
		GPIOA->CRH |= 0X00033000;
		PAout(12) = 0;
	}
}

void Set_System(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	EXTI_InitStructure.EXTI_Line = EXTI_Line18;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void Set_USBClock(void) {
	/* Select USBCLK source */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
 * Function Name  : Enter_LowPowerMode.
 * Description    : Power-off system clocks and power while entering suspend mode.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 ****************************************************************************** */
void Enter_LowPowerMode(void) {
	/* Set the device state to suspend */
	bDeviceState = SUSPENDED;
}

/*******************************************************************************
 * Function Name  : Leave_LowPowerMode.
 * Description    : Restores system clocks and power while exiting suspend mode.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 ****************************************************************************** */
void Leave_LowPowerMode(void) {
	DEVICE_INFO *pInfo = &Device_Info;

	/* Set the device state to the correct state */
	if (pInfo->Current_Configuration != 0) {
		/* Device configured */
		bDeviceState = CONFIGURED;
	}
	else {
		bDeviceState = ATTACHED;
	}

	/* Enable SystemCoreClock */
	SystemInit();
}

/*******************************************************************************
 * Function Name  : USB_Interrupts_Config.
 * Description    : Configures the USB interrupts.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 ****************************************************************************** */
void USB_Interrupts_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the USB interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USB Wake-up interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * Function Name : Joystick_Send.
 * Description   : prepares buffer to be sent containing Joystick event infos.
 * Input         : Keys: keys received from terminal.
 * Output        : None.
 * Return value  : None.
 ****************************************************************************** */
void Joystick_Send(uint8_t Key0, uint8_t Key1, uint8_t Key2, uint8_t Key3, uint8_t Key4, uint8_t Key5, uint8_t Key6) {
	uint8_t Buffer[8];
	Buffer[0] = Key0;
	Buffer[1] = 0;
	Buffer[2] = Key1;
	Buffer[3] = Key2;
	Buffer[4] = Key3;
	Buffer[5] = Key4;
	Buffer[6] = Key5;
	Buffer[7] = Key6;
	/* Copy mouse position info in ENDP1 Tx Packet Memory Area */
	USB_SIL_Write(EP1_IN, Buffer, 8);
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP1);
}

/*******************************************************************************
 * Function Name  : Get_SerialNum.
 * Description    : Create the serial number string descriptor.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 ****************************************************************************** */
void Get_SerialNum(void) {
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

	Device_Serial0 = *(uint32_t*)ID1;
	Device_Serial1 = *(uint32_t*)ID2;
	Device_Serial2 = *(uint32_t*)ID3;

	Device_Serial0 += Device_Serial2;

	if (Device_Serial0 != 0) {
		IntToUnicode(Device_Serial0, &Joystick_StringSerial[2], 8);
		IntToUnicode(Device_Serial1, &Joystick_StringSerial[18], 4);
	}
}

/*******************************************************************************
 * Function Name  : HexToChar.
 * Description    : Convert Hex 32Bits value into char.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 ****************************************************************************** */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len) {
	uint8_t idx = 0;

	for (idx = 0; idx < len; idx++) {
		if (((value >> 28)) < 0xA) {
			pbuf[2 * idx] = (value >> 28) + '0';
		}
		else {
			pbuf[2 * idx] = (value >> 28) + 'A' - 10;
		}

		value = value << 4;

		pbuf[2 * idx + 1] = 0;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
