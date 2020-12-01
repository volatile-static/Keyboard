/**
  ******************************************************************************
  * @file    platform_config.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
//#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#if defined (STM32F10X_MD) || defined (STM32F10X_HD) || defined (STM32F10X_XL)
 #include "stm32f10x.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
/*Unique Devices IDs register set*/

#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)
/* 
   For STM32L15xx devices it is possible to use the internal USB pullup
   controlled by register SYSCFG_PMC (refer to RM0038 reference manual for
   more details).
   It is also possible to use external pullup (and disable the internal pullup)
   by setting the define USB_USE_EXTERNAL_PULLUP in file platform_config.h
   and configuring the right pin to be used for the external pull up configuration.
   To have more details on how to use an external pull up, please refer to 
   STM3210E-EVAL evaluation board manuals.
   */
 /* Uncomment the following define to use an external pull up instead of the 
    integrated STM32L15xx internal pull up. In this case make sure to set up
    correctly the external required hardware and the GPIO defines below.*/
/* #define USB_USE_EXTERNAL_PULLUP */

 #if !defined(USB_USE_EXTERNAL_PULLUP)
  #define STM32L15_USB_CONNECT                SYSCFG_USBPuCmd(ENABLE)
  #define STM32L15_USB_DISCONNECT             SYSCFG_USBPuCmd(DISABLE)

 #elif defined(USB_USE_EXTERNAL_PULLUP)
  /* PA0 is chosen just as illustrating example, you should modify the defines
    below according to your hardware configuration. */
  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_0
  #define RCC_AHBPeriph_GPIO_DISCONNECT       RCC_AHBPeriph_GPIOA
  #define STM32L15_USB_CONNECT                GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN)
  #define STM32L15_USB_DISCONNECT             GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN)
 #endif /* USB_USE_EXTERNAL_PULLUP */
                                                
  #define RCC_AHBPeriph_ALLGPIO               (RCC_AHBPeriph_GPIOA \
                                               | RCC_AHBPeriph_GPIOB \
                                               | RCC_AHBPeriph_GPIOC \
                                               | RCC_AHBPeriph_GPIOD \
                                               | RCC_AHBPeriph_GPIOE \
										       | RCC_AHBPeriph_GPIOF \
 
