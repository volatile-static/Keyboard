/**	
 * |----------------------------------------------------------------------
 * | Copyright (c) 2016 Tilen Majerle
 * |  
 * | Permission is hereby granted, free of charge, to any person
 * | obtaining a copy of this software and associated documentation
 * | files (the "Software"), to deal in the Software without restriction,
 * | including without limitation the rights to use, copy, modify, merge,
 * | publish, distribute, sublicense, and/or sell copies of the Software, 
 * | and to permit persons to whom the Software is furnished to do so, 
 * | subject to the following conditions:
 * | 
 * | The above copyright notice and this permission notice shall be
 * | included in all copies or substantial portions of the Software.
 * | 
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * | OTHER DEALINGS IN THE SOFTWARE.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32_i2c.h"

/* I2C2 AF fix for F0xx */
#if !defined(GPIO_AF4_I2C2) 
#define GPIO_AF4_I2C2   GPIO_AF1_I2C2
#endif

/* Timeout value */
#define I2C_TIMEOUT_VALUE              1000

/* Handle values for I2C */
#ifdef I2C1
static I2C_HandleTypeDef I2C1Handle = {I2C1};
#endif
#ifdef I2C2
static I2C_HandleTypeDef I2C2Handle = {I2C2};
#endif
#ifdef I2C3
static I2C_HandleTypeDef I2C3Handle = {I2C3};
#endif
#ifdef I2C4
static I2C_HandleTypeDef I2C4Handle = {I2C4};
#endif

/* Private functions */
#ifdef I2C1
static void TM_I2C1_INT_InitPins(TM_I2C_PinsPack_t pinspack);
#endif
#ifdef I2C2
static void TM_I2C2_INT_InitPins(TM_I2C_PinsPack_t pinspack);
#endif
#ifdef I2C3
static void TM_I2C3_INT_InitPins(TM_I2C_PinsPack_t pinspack);
#endif
#ifdef I2C4
static void TM_I2C4_INT_InitPins(TM_I2C_PinsPack_t pinspack);
#endif

I2C_HandleTypeDef* TM_I2C_GetHandle(I2C_TypeDef* I2Cx) {
#ifdef I2C1
	if (I2Cx == I2C1) {
		return &I2C1Handle;
	}
#endif
#ifdef I2C2
	if (I2Cx == I2C2) {
		return &I2C2Handle;
	}
#endif
#ifdef I2C3
	if (I2Cx == I2C3) {
		return &I2C3Handle;
	}
#endif
#ifdef I2C4
	if (I2Cx == I2C4) {
		return &I2C4Handle;
	}
#endif
	
	/* Return invalid */
	return 0;
}

static void TM_I2C_FillSettings(I2C_HandleTypeDef* Handle, uint32_t clockSpeed) 
{
	/* Fill settings */
	Handle->Init.OwnAddress2 = 0x00;
	Handle->Init.OwnAddress1 = 0x00;
	Handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	Handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	Handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	Handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; 
}
//这个函数和MX_I2C1_Init()有啥区别？？？？为什么MX_I2C1_Init()不行？？？原先用F4，现在换L4，部分参数不同，为什么
//必须把void MX_I2C1_Init(void)里面的东西放到这里，用指针的形式再初始化一下？？
TM_I2C_Result_t TM_I2C_Init(I2C_TypeDef* I2Cx, TM_I2C_PinsPack_t pinspack, uint32_t clockSpeed) 
{	
  I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
  //	/* Fill instance value */
  Handle->Instance = I2C1; 
  Handle->Init.OwnAddress2 = 0x00;
  Handle->Init.OwnAddress1 = 0x00;
  Handle->Init.Timing = 0x00802D93;
  Handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  Handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  Handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  Handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; 
  //	/* Initialize I2C */
  HAL_I2C_Init(Handle); 
  /* Return OK */
  return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
	
	/* Send address */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, &register_address, 1, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(Handle, device_address, data, 1, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data, uint16_t count) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
	
	/* Send register address */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, &register_address, 1, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(Handle, device_address, data, count, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t* data) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);

	/* Receive single byte without specifying  */
	if (HAL_I2C_Master_Receive(Handle, (uint16_t)device_address, data, 1, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);

	/* Receive multi bytes without specifying  */
	if (HAL_I2C_Master_Receive(Handle, (uint16_t)device_address, data, count, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t data) {
	uint8_t d[2];
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
		
	/* Format array to send */
	d[0] = register_address;
	d[1] = data;
	
	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, (uint8_t *)d, 2, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	} 
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t* data, uint16_t count) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);

	/* Try to transmit via I2C */
	if (HAL_I2C_Mem_Write(Handle, device_address, register_address, register_address > 0xFF ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, data, count, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t data) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
	
	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, &data, 1, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	} 
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
	
	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, data, count, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	} 
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_Write16(I2C_TypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t data) {
	uint8_t d[3];
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
		
	/* Format array to send */
	d[0] = (register_address >> 8) & 0xFF; /* High byte */
	d[1] = (register_address) & 0xFF;      /* Low byte */
	d[2] = data;                           /* Data byte */
	
	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, (uint8_t *)d, 3, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	} 
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_Read16(I2C_TypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t* data) {
	uint8_t adr[2];
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
	
	/* Format I2C address */
	adr[0] = (register_address >> 8) & 0xFF; /* High byte */
	adr[1] = (register_address) & 0xFF;      /* Low byte */
	
	/* Send address */
	if (HAL_I2C_Master_Transmit(Handle, (uint16_t)device_address, adr, 2, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(Handle, device_address, data, 1, 1000) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(Handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t device_address) {
  I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
  /* Check if device is ready for communication */
  if (HAL_I2C_IsDeviceReady(&hi2c1, device_address, 2, 10) != HAL_OK) {
    /* Return error */
    return TM_I2C_Result_Error;
  }
  /* Return OK */
  return TM_I2C_Result_Ok;
}

TM_I2C_Result_t TM_I2C_WriteReadRepeatedStart(
	I2C_TypeDef* I2Cx,
	uint8_t device_address, 
	uint8_t write_register_address, 
	uint8_t* write_data,
	uint16_t write_count, 
	uint8_t read_register_address, 
	uint8_t* read_data,
	uint16_t read_count
) {
	I2C_HandleTypeDef* Handle = TM_I2C_GetHandle(I2Cx);
	
	/* Write command to device */
	if (HAL_I2C_Mem_Write(Handle, device_address, write_register_address, I2C_MEMADD_SIZE_8BIT, write_data, write_count, 1000) != HAL_OK) {
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Read data from controller */
	if (HAL_I2C_Mem_Read(Handle, device_address, read_register_address, I2C_MEMADD_SIZE_8BIT, read_data, read_count, 1000) != HAL_OK) {
		/* Return error */
		return TM_I2C_Result_Error;
	}
	
	/* Return OK */
	return TM_I2C_Result_Ok;
}

__weak void TM_I2C_InitCustomPinsCallback(I2C_TypeDef* I2Cx, uint16_t AlternateFunction) {
	/* Custom user function. */
	/* In case user needs functionality for custom pins, this function should be declared outside this library */
}

/* Private functions */
#ifdef I2C1
static void TM_I2C1_INT_InitPins(TM_I2C_PinsPack_t pinspack)
{
}
#endif
#ifdef I2C2
static void TM_I2C2_INT_InitPins(TM_I2C_PinsPack_t pinspack) 
{
}
#endif
#ifdef I2C3
static void TM_I2C3_INT_InitPins(TM_I2C_PinsPack_t pinspack)
{
}
#endif
#ifdef I2C4
static void TM_I2C4_INT_InitPins(TM_I2C_PinsPack_t pinspack) 
{
}
#endif
