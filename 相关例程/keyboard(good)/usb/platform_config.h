#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------ */
/* Exported constants -------------------------------------------------------- */
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
 used to run the example */
/* Unique Devices IDs register set */

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
 correctly the external required hardware and the GPIO defines below. */
/* #define USB_USE_EXTERNAL_PULLUP */
#endif /* USB_USE_EXTERNAL_PULLUP */

#define RCC_AHBPeriph_ALLGPIO               (RCC_AHBPeriph_GPIOA \
                                               | RCC_AHBPeriph_GPIOB \
                                               | RCC_AHBPeriph_GPIOC \
                                               | RCC_AHBPeriph_GPIOD \
                                               | RCC_AHBPeriph_GPIOE \
										       | RCC_AHBPeriph_GPIOF \


