#include "aip1638.h"
#include "pin_def.h"

void SPI2_init(void) {
		SPI_InitTypeDef SPI_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		SPI_Cmd(SPI2, DISABLE);
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //SPI主机 
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //发送接收8位帧结构    
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;  //时钟悬空低    
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //数据捕获于第1个时钟沿  
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //NSS信号由软件控制    
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //定义波特率预分频的值:波特率预分频值为16    
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;     
		SPI_InitStructure.SPI_CRCPolynomial = 7;   //CRC值计算的多项式    
		SPI_Init(SPI2, &SPI_InitStructure); 
		SPI_Cmd(SPI2, ENABLE);
}

u8 SPI2_ReadWriteByte(u8 Data) {
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
	}
	SPI_I2S_SendData(SPI2, Data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == RESET) {
	}
	return 0;
}

void aip1638_write(u8 Data)	{
		//SPI2_ReadWriteByte(Data);
		u8 i;
		for (i = 0; i < 8; ++i) {
			CLK = 0;                                                                   
			delay_us(1);
			DO = Data & 0X01;
			Data >>= 1;
			CLK = 1;                                                                   
			delay_us(1);
		}
}

void aip1638_readKey(u8 KeyPressed[24]) {
		u8 i, j;
		STB = 0;
		aip1638_write(0x42);
		delay_us(10);
		
		DO = 1;
		for (i = 0; i < 8; ++i) {
				for (j = 0; j < 3; ++j) {
						CLK = 0;
						delay_us(1);
						KeyPressed[i*3 + j] = DI;
						CLK = 1;
						delay_us(1);
				}
				CLK = 0;
				delay_us(1);
				CLK = 1;
				delay_us(1);
		}
		STB = 1;
}

void aip1638_writeCmd(u8 Cmd) {
	  STB = 0;                                                                      
    aip1638_write(Cmd);
		STB = 1;                                                                      
    delay_us(1);
}

void aip1638_init(void) {
		u8 i;
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOB, GPIO_Pin_12); 

		aip1638_writeCmd(0x8f);
		aip1638_writeCmd(0x40);
		STB = 0;
		aip1638_write(0xc0);
    for (i = 0; i < 16; i++){
        aip1638_write(0x00);
    }
		STB = 1;
}

void aip1638_writeData(u8 Addr, u8 Data) {
	  aip1638_writeCmd(0x44);
		STB = 0;
		aip1638_write(0xc0 | Addr);
		aip1638_write(Data);
		STB = 1;
}

void aip1638_setLED(u8 Dat[80]) {
		for(u8 i = 0; i < 8; ++i) {
				u8 t = 0;
				for (u8 j = 0; j < 8; ++j)
						t |= Dat[i*10 + j] << j;
				aip1638_writeData(i*2, t);
				aip1638_writeData(i*2 + 1, Dat[i*10 + 8]*2 | Dat[i*10 + 9]);
		}
}
