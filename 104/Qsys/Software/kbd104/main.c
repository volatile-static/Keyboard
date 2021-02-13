#include "key.h"
#include "led.h"
#include <unistd.h>

int main() {
	led_init();
	key_init();

	SET_LED(NUM_LOCK, 2);
	SET_LED(CAPS_LOCK, 22);
	SET_LED(SCR_LOCK, 222);

	while (1) {
		led_sched();
		// usleep(10000);
	}
}
