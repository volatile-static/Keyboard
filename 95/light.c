#ifdef RGBLIGHT_ENABLE
#include "quantum.h"
#include "debug.h"

#define LIGHT_PIN          B10
#define LIGHT_PWM_DRIVER   PWMD2
#define LIGHT_PWM_PAL_MODE 1
#define LIGHT_PWM_CHANNEL  3
#define LIGHT_DMA_CHANNEL  3
#define LIGHT_DMA_STREAM   STM32_DMA1_STREAM1
#define LIGHT_TRST_US      300

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define LIGHT_PWM_FREQUENCY (STM32_SYSCLK / 2)           /**< Clock frequency of PWM, must be valid with respect to system clock! */
#define LIGHT_PWM_PERIOD    (LIGHT_PWM_FREQUENCY / 800000) /**< Clock period in ticks. 1 / 800kHz = 1.25 uS (as per datasheet) */

/**
 * @brief   Number of bit-periods to hold the data line low at the end of a frame
 *
 * The reset period for each frame must be at least 50 uS; so we add in 50 bit-times
 * of zeroes at the end. (50 bits)*(1.25 uS/bit) = 62.5 uS, which gives us some
 * slack in the timing requirements
 */
#define LIGHT_COLOR_BITS  (24)
#define LIGHT_RESET_BIT_N (1000 * LIGHT_TRST_US / WS2812_TIMING)
#define LIGHT_COLOR_BIT_N ((RGBLED_NUM + 3) * LIGHT_COLOR_BITS) /**< Number of data bits */
#define LIGHT_BIT_N       (LIGHT_COLOR_BIT_N + LIGHT_RESET_BIT_N)     /**< Total number of bits in a frame */
#define LIGHT_OUTPUT_MODE (PAL_MODE_ALTERNATE(LIGHT_PWM_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST | PAL_PUPDR_FLOATING)

/**
 * @brief   High period for a zero, in ticks
 *
 * Per the datasheet:
 * WS2812:
 * - T0H: 200 nS to 500 nS, inclusive
 * - T0L: 650 nS to 950 nS, inclusive
 * WS2812B:
 * - T0H: 200 nS to 500 nS, inclusive
 * - T0L: 750 nS to 1050 nS, inclusive
 * SK6812:
 * - T0H: 150 nS to 450 nS, inclusive
 * - T0L: 750 nS to 1050 nS, inclusive
 *
 * The duty cycle is calculated for a high period of 350 nS.
 */
#define WS2812_DUTYCYCLE_0 16
// (LIGHT_PWM_FREQUENCY / (1000000000 / 400))

/**
 * @brief   High period for a one, in ticks
 *
 * Per the datasheet:
 * WS2812:
 * - T1H: 550 nS to 850 nS, inclusive
 * - T1L: 450 nS to 750 nS, inclusive
 * WS2812B:
 * - T1H: 750 nS to 1050 nS, inclusive
 * - T1L: 200 nS to 500 nS, inclusive
 * SK6812:
 * - T1H: 450 nS to 750 nS, inclusive
 * - T1L: 450 nS to 750 nS, inclusive
 *
 * The duty cycle is calculated for a high period of 800 nS.
 * This is in the middle of the specifications of the WS2812 and WS2812B.
 */
#define WS2812_DUTYCYCLE_1 26
// (LIGHT_PWM_FREQUENCY / (1000000000 / 500))

/* --- PRIVATE MACROS ------------------------------------------------------- */

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given bit
 *
 * @param[in] led:                  The led index [0, @ref WS2812_LED_N)
 * @param[in] byte:                 The byte number [0, 2]
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BIT(led, byte, bit) (LIGHT_COLOR_BITS * (led) + 8 * (byte) + (7 - (bit)))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given red bit
 *
 * @note    The red byte is the middle byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref WS2812_LED_N)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_RED_BIT(led, bit) WS2812_BIT((led), 1, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given green bit
 *
 * @note    The green byte is the first byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref WS2812_LED_N)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_GREEN_BIT(led, bit) WS2812_BIT((led), 0, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given blue bit
 *
 * @note    The blue byte is the last byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref WS2812_LED_N)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BLUE_BIT(led, bit) WS2812_BIT((led), 2, (bit))

// Used for 32-bit timer
#define WS2812_DMA_MEMORY_WIDTH STM32_DMA_CR_MSIZE_WORD
#define WS2812_DMA_PERIPHERAL_WIDTH STM32_DMA_CR_PSIZE_WORD

/* --- PRIVATE VARIABLES ---------------------------------------------------- */
static uint32_t light_frame_buffer[LIGHT_BIT_N + 1]; /**< Buffer for a frame */

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/*
 * Gedanke: Double-buffer type transactions: double buffer transfers using two memory pointers for
the memory (while the DMA is reading/writing from/to a buffer, the application can
write/read to/from the other buffer).
 */

void light_init(void) {
    // Initialize led frame buffer
    uint32_t i;
    for (i = 0; i < LIGHT_COLOR_BIT_N; i++)
        light_frame_buffer[i] = WS2812_DUTYCYCLE_0; // All color bits are zero duty cycle
    for (i = 0; i < LIGHT_RESET_BIT_N; i++)
        light_frame_buffer[i + LIGHT_COLOR_BIT_N] = 0; // All reset bits are zero

    palSetLineMode(LIGHT_PIN, LIGHT_OUTPUT_MODE); // Set pin to output mode

    // PWM Configuration
    static const PWMConfig ws2812_pwm_config = {
        .frequency = LIGHT_PWM_FREQUENCY,
        .period    = LIGHT_PWM_PERIOD, // Mit dieser Periode wird UDE-Event erzeugt und ein neuer Wert (Länge LIGHT_BIT_N) vom DMA ins CCR geschrieben
        .callback  = NULL,
        .channels =
            {
                [0 ... 3]               = {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},    // Channels default to disabled
                [LIGHT_PWM_CHANNEL - 1] = {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL}, // Turn on the channel we care about
            },
        .cr2  = 0,
        .dier = TIM_DIER_UDE, // DMA on update event for next period
    };

    // Configure DMA
    const stm32_dma_stream_t *dma = dmaStreamAlloc(STM32_DMA_STREAM_ID(1, 1), 10, NULL, NULL);
    dmaStreamSetPeripheral(dma, &(LIGHT_PWM_DRIVER.tim->CCR[LIGHT_PWM_CHANNEL - 1]));
    dmaStreamSetMemory0(dma, light_frame_buffer);
    dmaStreamSetMode(dma, STM32_DMA_CR_CHSEL(LIGHT_DMA_CHANNEL) 
        | STM32_DMA_CR_DIR_M2P 
        | WS2812_DMA_PERIPHERAL_WIDTH 
        | WS2812_DMA_MEMORY_WIDTH 
        | STM32_DMA_CR_MINC 
        | STM32_DMA_CR_CIRC 
        | STM32_DMA_CR_PL(3)
    );
    dmaStreamSetTransactionSize(dma, LIGHT_BIT_N);
    // M2P: Memory 2 Periph; PL: Priority Level

    // Start DMA
    dmaStreamEnable(dma);

    // Configure PWM
    // NOTE: It's required that preload be enabled on the timer channel CCR register. This is currently enabled in the
    // ChibiOS driver code, so we don't have to do anything special to the timer. If we did, we'd have to start the timer,
    // disable counting, enable the channel, and then make whatever configuration changes we need.
    pwmStart(&LIGHT_PWM_DRIVER, &ws2812_pwm_config);
    pwmEnableChannel(&LIGHT_PWM_DRIVER, LIGHT_PWM_CHANNEL, 0); // Initial period is 0; output will be low until first duty cycle is DMA'd in
}

void light_write_led(uint32_t led_number, uint8_t r, uint8_t g, uint8_t b) {
    // Write color to frame buffer
    for (uint8_t bit = 0; bit < 8; ++bit) {
        light_frame_buffer[WS2812_RED_BIT(led_number, bit)]   = ((r >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
        light_frame_buffer[WS2812_GREEN_BIT(led_number, bit)] = ((g >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
        light_frame_buffer[WS2812_BLUE_BIT(led_number, bit)]  = ((b >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
    }
}

// override
void rgblight_call_driver(LED_TYPE *led_array, uint8_t num_leds) {
    for (uint16_t i = 0; i < num_leds; i++) 
        light_write_led(i + 3, led_array[i].r, led_array[i].g, led_array[i].b);
}

#endif
