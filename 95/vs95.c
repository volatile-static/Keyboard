#include "quantum.h"
#include "color.h"
#include "ws2812.h"
#include "light.h"

void keyboard_post_init_kb(void) {
    setPinOutput(A13);
    setPinOutput(A14);
    debug_config.enable = debug_config.matrix = debug_config.keyboard = true;
    light_init();
    rgb_matrix_mode(RGB_MATRIX_TYPING_HEATMAP);
    rgblight_mode(RGBLIGHT_MODE_TWINKLE_end);

    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    togglePin(A14);
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    return process_record_user(keycode, record);
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    if (get_highest_layer(state)) 
        rgblight_mode(RGBLIGHT_MODE_SNAKE_end);
    else
        rgblight_mode(RGBLIGHT_MODE_TWINKLE_end);
    return layer_state_set_user(state);
}

bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);
    HSV val = rgb_matrix_get_hsv();
    RGB rgb = hsv_to_rgb(val);
    if (res) {
        if (led_state.num_lock)
            light_write_led(LED_NUM_LOCK, rgb.r, rgb.g, rgb.b);
        else
            light_write_led(LED_NUM_LOCK, RGB_OFF);

        if (led_state.caps_lock)
            light_write_led(LED_CAPS_LOCK, rgb.r, rgb.g, rgb.b);
        else
            light_write_led(LED_CAPS_LOCK, RGB_OFF);

        if (led_state.scroll_lock)
            light_write_led(LED_SCROLL_LOCK, rgb.r, rgb.g, rgb.b);
        else
            light_write_led(LED_SCROLL_LOCK, RGB_OFF);
    }
    return res;
}
