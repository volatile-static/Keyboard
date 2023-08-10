enum led_indicators_t {
    LED_SCROLL_LOCK = 0, LED_CAPS_LOCK, LED_NUM_LOCK
};

void light_init(void); 
void light_write_led(uint32_t led_number, uint8_t r, uint8_t g, uint8_t b);
