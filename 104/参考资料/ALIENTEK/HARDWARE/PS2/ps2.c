#include "ps2.h" 
#include "delay.h"
#include <stdio.h>

//Enable serial debug mode?
//#define _PS2DBG Serial

//since for the device side we are going to be in charge of the clock,
//the two defines below are how long each _phase_ of the clock cycle is
#define CLKFULL 40
// we make changes in the middle of a phase, this how long from the
// start of phase to the when we drive the data line
#define CLKHALF 20

// Delay between bytes
// I've found i need at least 400us to get this working at all,
// but even more is needed for reliability, so i've put 1000us
#define BYTEWAIT 1000

// Timeout if computer not sending for 30ms
#define TIMEOUT 30

#define RETURN_IF(s) if (s) { \
                        printf("check failed at %d \n", __LINE__);    \
                        return false;                   \
                    }

void ps2_wait_for_clock(void) {
	PS2_SCL_OUT = 1;
	while (!PS2_SCL)
		continue;
}

static inline void ps2_send_bit(bool bit) {
	PS2_SDA_OUT = bit;
	delay_us(CLKHALF);
	PS2_SCL_OUT = 0;
	delay_us(CLKFULL);
	PS2_SCL_OUT = 1;
	delay_us(CLKHALF);
}

bool ps2_send_byte(uint8_t byte) {
	ps2_wait_for_clock();
	delay_us(50);
	RETURN_IF(!PS2_SDA); // ?
	ps2_send_bit(0);
	RETURN_IF(!PS2_SCL);

	uint8_t parity = 1;
	for (int i = 0; i < 8; ++i) {
		parity ^= byte & 0x01;
		ps2_send_bit(byte & 0x01);
		byte >>= 1;
		RETURN_IF(!PS2_SCL);
	}

	ps2_send_bit(parity);
	RETURN_IF(!PS2_SCL);
	ps2_send_bit(1);
	PS2_SCL_OUT = PS2_SDA_OUT = 1;
	return true;
}

static inline bool ps2_recv_bit(void) {
	delay_us(CLKHALF);
	PS2_SCL_OUT = 0;
	delay_us(CLKFULL);
	PS2_SCL_OUT = 1;
	delay_us(CLKHALF);
	return PS2_SDA;
}

static inline void ps2_send_ack(void) {
	delay_us(CLKHALF);
	PS2_SDA_OUT = 0;
	delay_us(1);
	PS2_SCL_OUT = 0;
	delay_us(CLKFULL);
	PS2_SCL_OUT = 1;
	delay_us(CLKHALF);
	PS2_SDA_OUT = 1;
}

uint16_t ps2_recv_byte(void) {
	uint8_t ret = 0, tmp, parity = 0;
	PS2_SCL_OUT = PS2_SDA_OUT = 1;
	for (int i = 0; !PS2_SCL; ++i)
		if (i > 555)
			return 0x21e;
	RETURN_IF(PS2_SDA);

	for (int i = 0; i < 8; ++i) {
		tmp = ps2_recv_bit();
		if (!PS2_SCL)
			return 0xBAD;
		ret >>= 1;
		if (tmp) {
			ret |= 0x80;
			++parity;
		}
	}

	bool f = ps2_recv_bit();
	if (f != (parity & 0x01))
		printf("parity %d , read %d , bin %x\n", parity, f, ret);
	//		return 0xE22;
	if (!ps2_recv_bit()) {
		while (!ps2_recv_bit())
			continue;
		return 0x71e;
	}
	ps2_send_ack();
	return ret;
}

void ps2_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化指定端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	//PC5
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化指定端口	
}

//在发送命令/数据之后,等待设备应带,该函数用来获取应答
//返回得到的值 
//返回0，且PS2_Status.6=1，则产生了错误
//CHECK OK 2010/5/2
// u8 PS2_Get_Byte(void)
// {
// 	u16 t=0;
// 	u8 temp=0;
// 	while(1)//最大等待55ms
// 	{
// 		t++;
// 		delay_us(10);
// 		if(PS2_Status&0x80)//得到了一次数据
// 		{
// 			temp=PS2_DATA_BUF[PS2_Status&0x0f-1];
// 			PS2_Status&=0x70;//清除计数器，接收到数据标记
// 			break;	
// 		}else if(t>5500||PS2_Status&0x40)break;//超时溢出/接收错误	   
// 	}
// 	PS2_En_Data_Report();//使能数据传输
// 	return temp;    
// }	    
//发送一个命令到PS2.
//返回值:0，无错误,其他,错误代码
// u8 PS2_Send_Cmd(u8 cmd)
// {
// 	u8 i;
// 	u8 high=0;//记录1的个数		 
// 	PS2_Set_Int(0);   //屏蔽中断
// 	PS2_SET_SCL_OUT();//设置SCL为输出
// 	PS2_SET_SDA_OUT();//SDA OUT
// 	PS2_SCL_OUT=0;//拉低时钟线
// 	delay_us(120);//保持至少100us
// 	PS2_SDA_OUT=0;//拉低数据线
// 	delay_us(10);
// 	PS2_SET_SCL_IN();//释放时钟线,这里PS2设备得到第一个位,开始位
// 	PS2_SCL_OUT=1;
// 	if(Wait_PS2_Scl(0)==0)//等待时钟拉低
// 	{									  
// 		for(i=0;i<8;i++)
// 		{
// 			if(cmd&0x01)
// 			{
// 			    PS2_SDA_OUT=1;
// 				high++;
// 			}else PS2_SDA_OUT=0;   
// 			cmd>>=1;
// 			//这些地方没有检测错误,因为这些地方不会产生死循环
// 			Wait_PS2_Scl(1);//等待时钟拉高 发送8个位
// 			Wait_PS2_Scl(0);//等待时钟拉低
// 		}
// 		if((high%2)==0)PS2_SDA_OUT=1;//发送校验位 10
// 		else PS2_SDA_OUT=0;
// 		Wait_PS2_Scl(1); //等待时钟拉高 10位
// 		Wait_PS2_Scl(0); //等待时钟拉低
// 		PS2_SDA_OUT=1;   //发送停止位  11	  
//  		Wait_PS2_Scl(1);//等待时钟拉高 11位
// 		PS2_SET_SDA_IN();//SDA in
// 		Wait_PS2_Scl(0);//等待时钟拉低
// 		if(PS2_SDA==0)Wait_PS2_Scl(1);//等待时钟拉高 12位 
// 		else 
// 		{
// 			PS2_En_Data_Report();
// 			return 1;//发送失败
// 		}		
// 	}else 
// 	{
// 		PS2_En_Data_Report();
// 		return 2;//发送失败
// 	}
// 	PS2_En_Data_Report();
// 	return 0;    //发送成功
// }















