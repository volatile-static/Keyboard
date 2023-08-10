// Copyright 2023 ZJU (@VS)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

// #define WS2812_T1H 720
// #define WS2812_T1L 355
// #define WS2812_T0H 355
// #define WS2812_T0L 680
// #define WS2812_TIMING 900

#define WS2812_PWM_DRIVER PWMD3
#define WS2812_DMA_CHANNEL 5
#define WS2812_TRST_US 160
#define WS2812_PWM_TARGET_PERIOD 1123595

#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define RGB_MATRIX_LED_COUNT 94
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES
