/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "tm_stm32_usb_hid_device.h"
#include "multi_button.h"
#include "tm_stm32_mpu9250.h"
#include "tm_stm32_ahrs_imu.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

uint32_t cnt1=0,cnt2=0;
struct Button UserBtn,key1,key2,key3,key4,key5,key6,key7,key8;
uint8_t keyflag=0;//total 8 keys
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define KEY_W 0X1A 
#define KEY_S 0X16
#define KEY_A 0X04
#define KEY_D 0X07
#define KEY_R 0X15
#define KEY_F 0X09

#define USEDEBUG 1

uint8_t UserBtnflag=0;
uint8_t AHRS_DataReady=0;

TM_MPU9250_t MPU9250;
TM_AHRSIMU_t IMU;

int roll=0,pith=0,yaw=0;

/* Set structs for all examples */
TM_USB_HIDDEVICE_Keyboard_t Keyboard;
TM_USB_HIDDEVICE_Gamepad_t Gamepad1, Gamepad2;
TM_USB_HIDDEVICE_Mouse_t Mouse;

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_INT_Status;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
uint8_t read_UserBtn_GPIO() 
{
    return HAL_GPIO_ReadPin(UserBtn_GPIO_Port, UserBtn_Pin);
}
uint8_t read_key1_GPIO() 
{
    return HAL_GPIO_ReadPin(key1_GPIO_Port, key1_Pin);
}
uint8_t read_key2_GPIO() 
{
    return HAL_GPIO_ReadPin(key2_GPIO_Port, key2_Pin);
}
uint8_t read_key3_GPIO() 
{
    return HAL_GPIO_ReadPin(key3_GPIO_Port, key3_Pin);
}
uint8_t read_key4_GPIO() 
{
    return HAL_GPIO_ReadPin(key4_GPIO_Port, key4_Pin);
}
uint8_t read_key5_GPIO() 
{
    return HAL_GPIO_ReadPin(key5_GPIO_Port, key5_Pin);
}
uint8_t read_key6_GPIO() 
{
    return HAL_GPIO_ReadPin(key6_GPIO_Port, key6_Pin);
}
uint8_t read_key7_GPIO() 
{
    return HAL_GPIO_ReadPin(key7_GPIO_Port, key7_Pin);
}
uint8_t read_key8_GPIO() 
{
    return HAL_GPIO_ReadPin(key8_GPIO_Port, key8_Pin);
}
//UserBtn
void UserBtn_PRESS_DOWN_Handler(void* btn)
{
  //do something...
#ifdef USEDEBUG
  printf("UserBtn_PRESS_DOWN!\n");
#endif
}

void UserBtn_PRESS_UP_Handler(void* btn)
{
  //do something...
#ifdef USEDEBUG
  printf("UserBtn_PRESS_UP!\n");
#endif
}
void UserBtn_SINGLE_Click_Handler(void* btn)
{
  //do something...
  UserBtnflag++;
  if(UserBtnflag>2)
  {
    UserBtnflag=0;
  }
#ifdef USEDEBUG
  printf("UserBtn_SINGLE_Click!\n");
#endif
}
//key1
void key1_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT0;
  //do something...
#ifdef USEDEBUG
  printf("key1_PRESS_DOWN!\n");
#endif
}

void key1_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT0;
  //do something...
#ifdef USEDEBUG
  printf("key1_PRESS_UP!\n");
#endif
}
//key2
void key2_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT1;
  //do something...
#ifdef USEDEBUG
  printf("key2_PRESS_DOWN!\n");
#endif
}

void key2_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT1;
  //do something...
#ifdef USEDEBUG
  printf("key2_PRESS_UP!\n");
#endif
}
//key3
void key3_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT2;
  
    //do something...
  #ifdef USEDEBUG
	printf("key3_PRESS_DOWN!\n");
  #endif
}

void key3_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT2;
  //do something...
#ifdef USEDEBUG
  printf("key3_PRESS_UP!\n");
#endif
}

//key4
void key4_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT3;
  
    //do something...
  #ifdef USEDEBUG
	printf("key4_PRESS_DOWN!\n");
  #endif
}
void key4_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT3;
  //do something...
#ifdef USEDEBUG
  printf("key4_PRESS_UP!\n");
#endif
}

//key5
void key5_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT4;
  
    //do something...
  #ifdef USEDEBUG
	printf("key5_PRESS_DOWN!\n");
  #endif
}
void key5_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT4;
  //do something...
#ifdef USEDEBUG
  printf("key5_PRESS_UP!\n");
#endif
}

//key6
void key6_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT5;
  
    //do something...
  #ifdef USEDEBUG
	printf("key6_PRESS_DOWN!\n");
  #endif
}
void key6_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT5;
  //do something...
#ifdef USEDEBUG
  printf("key6_PRESS_UP!\n");
#endif
}

//key7
void key7_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT6;
  
    //do something...
  #ifdef USEDEBUG
	printf("key7_PRESS_DOWN!\n");
  #endif
}
void key7_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT6;
  //do something...
#ifdef USEDEBUG
  printf("key7_PRESS_UP!\n");
#endif
}

//key8
void key8_PRESS_DOWN_Handler(void* btn)
{
  keyflag|=BIT7;
  
    //do something...
  #ifdef USEDEBUG
	printf("key8_PRESS_DOWN!\n");
  #endif
}
void key8_PRESS_UP_Handler(void* btn)
{
  keyflag&=~BIT7;
  //do something...
#ifdef USEDEBUG
  printf("key8_PRESS_UP!\n");
#endif
}
void UserKeys_Init(void)
{
  button_init(&UserBtn, read_UserBtn_GPIO, 0);//key is Low voltage valid
  button_attach(&UserBtn, PRESS_DOWN,       UserBtn_PRESS_DOWN_Handler);
  button_attach(&UserBtn, PRESS_UP,         UserBtn_PRESS_UP_Handler); 
  button_attach(&UserBtn, SINGLE_CLICK,     UserBtn_SINGLE_Click_Handler);
  button_start(&UserBtn);
  
  button_init(&key1, read_key1_GPIO, 0);//key is Low voltage valid
  button_attach(&key1, PRESS_DOWN,  key1_PRESS_DOWN_Handler);
  button_attach(&key1, PRESS_UP,    key1_PRESS_UP_Handler); 
  button_start(&key1);
  
  button_init(&key2, read_key2_GPIO, 0);//key is Low voltage valid
  button_attach(&key2, PRESS_DOWN, key2_PRESS_DOWN_Handler);
  button_attach(&key2, PRESS_UP,   key2_PRESS_UP_Handler); 
  button_start(&key2);
  
  button_init(&key3, read_key3_GPIO, 0);//key is Low voltage valid
  button_attach(&key3, PRESS_DOWN, key3_PRESS_DOWN_Handler);
  button_attach(&key3, PRESS_UP,   key3_PRESS_UP_Handler); 
  button_start(&key3);
  
  button_init(&key4, read_key4_GPIO, 0);//key is Low voltage valid
  button_attach(&key4, PRESS_DOWN, key4_PRESS_DOWN_Handler);
  button_attach(&key4, PRESS_UP,   key4_PRESS_UP_Handler); 
  button_start(&key4);
  
  button_init(&key5, read_key5_GPIO, 0);//key is Low voltage valid
  button_attach(&key5, PRESS_DOWN, key5_PRESS_DOWN_Handler);
  button_attach(&key5, PRESS_UP,   key5_PRESS_UP_Handler); 
  button_start(&key5);
  
  button_init(&key6, read_key6_GPIO, 0);//key is Low voltage valid
  button_attach(&key6, PRESS_DOWN, key6_PRESS_DOWN_Handler);
  button_attach(&key6, PRESS_UP,   key6_PRESS_UP_Handler); 
  button_start(&key6);
  
  button_init(&key7, read_key7_GPIO, 0);//key is Low voltage valid
  button_attach(&key7, PRESS_DOWN, key7_PRESS_DOWN_Handler);
  button_attach(&key7, PRESS_UP,   key7_PRESS_UP_Handler); 
  button_start(&key7);
  
  button_init(&key8, read_key8_GPIO, 0);//key is Low voltage valid
  button_attach(&key8, PRESS_DOWN, key8_PRESS_DOWN_Handler);
  button_attach(&key8, PRESS_UP,   key8_PRESS_UP_Handler); 
  button_start(&key8);
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance ==TIM6)//tim6 5ms
  {
    button_ticks();//recommand 5ms for key scan
  }
  if(htim->Instance ==TIM7)//tim7  //recommand 15ms 
  {
    if (TM_MPU9250_DataReady(&MPU9250) == TM_MPU9250_Result_Ok) 
    {
      TM_MPU9250_ReadAcce(&MPU9250);
      TM_MPU9250_ReadGyro(&MPU9250);
      TM_MPU9250_ReadMag(&MPU9250);              
      TM_AHRSIMU_UpdateAHRS(&IMU, MPU9250.Gx, MPU9250.Gy, MPU9250.Gz,
                            MPU9250.Ax, MPU9250.Ay, MPU9250.Az,
                            MPU9250.Mx, MPU9250.My, MPU9250.Mz);
      AHRS_DataReady=1;
    }
    
  }
}
void LEDFlashing(uint32_t periodtime,uint32_t ontime)
{
  static uint32_t StartTime=0;
  static uint8_t LedStatus=0;
  if(!LedStatus) 
  {
    if(HAL_GetTick()-StartTime > periodtime) 
    {
      LD2_ON();
      LedStatus =1;
      StartTime = HAL_GetTick();
    }
  } 
  else 
  {
    if(HAL_GetTick()-StartTime > ontime) 
    {
      LD2_OFF();
      LedStatus =0;
      StartTime = HAL_GetTick();
    }
  }
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM6_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();

  /* USER CODE BEGIN 2 */
  printf("Hello STM32!!!\r\n");
  
  /* Init MPU9250 */
  if (TM_MPU9250_Init(&MPU9250, TM_MPU9250_Device_0) != TM_MPU9250_Result_Ok)
  {
    while (1)
    {
      printf("MPU9250 Device error!\r\n");
      HAL_Delay(500);
    }
  } 
  printf("Device connected!\r\n");   
  TM_AHRSIMU_Init(&IMU, 2000, 0.1, 0);
  
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  
  /*Init user keys*/
  UserKeys_Init();
  
  /* Initialize USB HID Device */
  TM_USB_HIDDEVICE_Init();
  /* Set default values for mouse struct */
  TM_USB_HIDDEVICE_MouseStructInit(&Mouse);
  /* Set default values for keyboard struct */
  TM_USB_HIDDEVICE_KeyboardStructInit(&Keyboard);
  /* Set default values for gamepad structs */
  TM_USB_HIDDEVICE_GamepadStructInit(&Gamepad1);
  TM_USB_HIDDEVICE_GamepadStructInit(&Gamepad2);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    
    if(UserBtnflag)//进入状态
    {  
      if(UserBtnflag==2)//初始化后按2下UserBtn，启用重力感应；板载LD2绿色LED 500ms亮，500ms灭
      {
        LEDFlashing(1000,500);
        if(AHRS_DataReady==1)
        {
          roll=(int)IMU.Roll;
          pith=(int)IMU.Pitch;
          yaw=(int)IMU.Yaw;       
          AHRS_DataReady=0;
        }     
        //printf("R: %d, P: %d, Y: %d\n", roll, pith, yaw);
        //HAL_Delay(200);     
        if(pith>=12)//前进
        {
          keyflag|=BIT1;
        }
        else if(pith<=0)//后退
        {
          keyflag|=BIT3;
        }
        else
        {
          keyflag&=~0x0a;
        }      
        if( roll>=145 && roll<=168 )//左转
        {
          keyflag|=BIT4;
        }
        else if( roll>=-170 && roll<=-145 )//右转
        {
          keyflag|=BIT5;
        }
        else
        {
          keyflag&=~0x30;
        } 
      }
      else
      {
        LEDFlashing(1000,200);//只启用按键，板载LD2绿色LED 200ms亮，800ms灭
      }
      if (TM_USB_HIDDEVICE_GetStatus() == TM_USB_HIDDEVICE_Status_Connected) 
      {
        if(keyflag)
        {
          if(keyflag&BIT0)//k1
          {
            Keyboard.Key1 = KEY_R;          
          }
          else
          {
            Keyboard.Key1 = 0;
          }
          if(keyflag&BIT1)//k2
          {
            Keyboard.Key2 = KEY_W;             
          }
          else
          {
            Keyboard.Key2 = 0;           
          }
          if(keyflag&BIT2)//k3
          {
            //Keyboard.Key3 = KEY_F;  
            Keyboard.L_SHIFT=TM_USB_HIDDEVICE_Button_Pressed;
          }
          else
          {
            Keyboard.L_SHIFT=TM_USB_HIDDEVICE_Button_Released;
           // Keyboard.Key3 = 0;
          }
          if(keyflag&BIT3)//k4
          {
            Keyboard.Key4 = KEY_S; 
          }
          else
          {
            Keyboard.Key4 = 0;
          }
          if(keyflag&BIT4)//k5
          {
            Keyboard.Key5= KEY_A; 
          }
          else
          {
            Keyboard.Key5 = 0;
          }
          if(keyflag&BIT5)//k6
          {
            Keyboard.Key6 = KEY_D; 
          }
          else
          {
            Keyboard.Key6 = 0;
          }

          if(keyflag&BIT6)//按键7设为鼠标左键单击
          {
            Mouse.LeftButton = TM_USB_HIDDEVICE_Button_Pressed; 
          }
          else
          {
            Mouse.LeftButton = TM_USB_HIDDEVICE_Button_Released; 
          }
          if(keyflag&BIT7)//按键8设为鼠标右键单击
          {
            Mouse.RightButton = TM_USB_HIDDEVICE_Button_Pressed; 
          }
          else
          {
            Mouse.RightButton = TM_USB_HIDDEVICE_Button_Released; 
          }
          TM_USB_HIDDEVICE_MouseSend(&Mouse);
          TM_USB_HIDDEVICE_KeyboardSend(&Keyboard); /* Send keyboard report */
        }
        else        
        {
          TM_USB_HIDDEVICE_MouseReleaseAll();
          TM_USB_HIDDEVICE_KeyboardReleaseAll(); 
        }
      }
    }   
    else
    {
      TM_USB_HIDDEVICE_KeyboardReleaseAll();
      LEDFlashing(1000,50);//未进入状态，板载LD2绿色LED快闪
    } 
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
