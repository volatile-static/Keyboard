#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */
/*******************************************************************************
 * Function Name  : EP1_OUT_Callback.
 * Description    : EP1 OUT Callback Routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 ****************************************************************************** */
void EP1_IN_Callback(void) {
}

void EP1_OUT_Callback(void) {
	u8 DataLen; //保存接收数据的长度
	u8 DataBuffer[64]; //保存接收数据的缓冲区
	DataLen = GetEPRxCount(ENDP1); //获取收到的长度
	PMAToUserBufferCopy(DataBuffer, ENDP1_RXADDR, DataLen); //复制数据
	SetEPRxValid(ENDP1); //设置端点有效，以接收下一次数据
	if(DataLen==1) //收到一字节的输出报告
	{
		//D0位表示数字键盘灯，D1位表示大写字母锁定灯
		if(DataBuffer[0]&0x01)  //数字键盘灯亮
		{
		// GPIOC->BSRR=(1<<6); //亮LED3
		}
		else
		{
		//GPIOC->BRR=(1<<6); //灭LED3
		}
		if(DataBuffer[0]&0x02) //大写字母锁定键
		{
		//GPIOC->BSRR=(1<<7); //亮LED2
		}
		else
		{
		//GPIOC->BRR=(1<<7); //灭LED2
		}
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
