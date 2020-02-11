#include "rgb.h"
#include "mole.h"
#include "keys.h"
#include "leds.h"
#include "app_timer.h"
#include "nrf_drv_rng.h"
#include "app_scheduler.h"

static uint8_t MoleLocation;
APP_TIMER_DEF(MOLE_TIM);

void mole_timeoutEvent(void *p_event_data, uint16_t event_size) {
    fl3236_setRGB(MoleLocation, 0, 0, 0x0c);
    while (nrf_drv_rng_rand(&MoleLocation, 1) != NRF_SUCCESS) {
    }
    MoleLocation %= 17;
    fl3236_setRGB(MoleLocation, 0x06, 0x0f, 0);
    fl3236_updateReg();
}

static void mole_timeout_handler(void *p) {
    app_sched_event_put(NULL, 0, mole_timeoutEvent);
}

void mole_init(void) {
    ret_code_t err_code;
    
	err_code = nrf_drv_rng_init(NULL);
    APP_ERROR_CHECK(err_code);
	err_code = app_timer_create(&MOLE_TIM, APP_TIMER_MODE_REPEATED, mole_timeout_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(MOLE_TIM, APP_TIMER_TICKS(1200), NULL);
    APP_ERROR_CHECK(err_code);
    
    for (uint8_t i = 0; i < 17; ++i)
        fl3236_setRGB(i, 0, 0, 0x0c);
    fl3236_updateReg();
}
