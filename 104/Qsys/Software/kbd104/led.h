#ifndef LED_H_
#define LED_H_

#include <io.h>

#define LED_MATRIX_BASE (0x1800u)
#define SET_LED(id, val) IOWR(LED_MATRIX_BASE, id, val)

#define ICE_BEGIN 0
#define ICE_END 72
#define FN_ID 73
#define RGB_BEGIN 74
#define RGB_END 103
#define NUM_LOCK 104
#define CAPS_LOCK 105
#define SCR_LOCK 106

#define MAX_BRIGHTNESS 0xfa
#define COLOR_DAT_SIZE (MAX_BRIGHTNESS*6u)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef union {
	struct {
		unsigned char red, green, blue;
	} color;
	unsigned argb;
} rgb_t;

void led_init(void);
void led_sched(void);

#endif /* LED_H_ */
