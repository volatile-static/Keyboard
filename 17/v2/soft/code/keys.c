#include "keys.h"
#include "i2c.h"
#include "rgb.h"
#include "boards.h"
#include "nrf_log.h"
#include "app_timer.h"
#include "nrfx_gpiote.h"
#include "app_scheduler.h"

#define TCA8418_ADDR (0x68 >> 1)

#define REG_CFG 0x01
#define REG_INT_STAT  0x02
#define REG_KEY_LCK_EC 0x03
#define REG_KEY_EVENT_A 0x04
#define REG_KEY_EVENT_B 0x05
#define REG_KEY_EVENT_C 0x06
#define REG_KEY_EVENT_D 0x07
#define REG_KEY_EVENT_E 0x08
#define REG_KEY_EVENT_F 0x09
#define REG_KEY_EVENT_G 0x0A
#define REG_KEY_EVENT_H 0x0B
#define REG_KEY_EVENT_I 0x0C
#define REG_KEY_EVENT_J 0x0D
#define REG_KP_LCK_TIMER 0x0E
#define REG_UNLOCK1 0x0F
#define REG_UNLOCK2 0x10
#define REG_GPIO_INT_STAT1 0x11
#define REG_GPIO_INT_STAT2 0x12
#define REG_GPIO_INT_STAT3 0x13
#define REG_GPIO_DAT_STAT1 0x14
#define REG_GPIO_DAT_STAT2 0x15
#define REG_GPIO_DAT_STAT3 0x16
#define REG_GPIO_DAT_OUT1 0x17
#define REG_GPIO_DAT_OUT2 0x18
#define REG_GPIO_DAT_OUT3 0x19
#define REG_GPIO_INT_EN1 0x1A
#define REG_GPIO_INT_EN2 0x1B
#define REG_GPIO_INT_EN3 0x1C
#define REG_KP_GPIO1 0x1D
#define REG_KP_GPIO2 0x1E
#define REG_KP_GPIO3 0x1F
#define REG_GPI_EM1	0x20
#define REG_GPI_EM2	0x21
#define REG_GPI_EM3	0x22
#define REG_GPIO_DIR1 0x23
#define REG_GPIO_DIR2 0x24
#define REG_GPIO_DIR3 0x25
#define REG_GPIO_INT_LVL1 0x26
#define REG_GPIO_INT_LVL2 0x27
#define REG_GPIO_INT_LVL3 0x28
#define REG_DEBOUNCE_DIS1 0x29
#define REG_DEBOUNCE_DIS2 0x2A
#define REG_DEBOUNCE_DIS3 0x2B
#define REG_GPIO_PULL1 0x2C
#define REG_GPIO_PULL2 0x2D
#define REG_GPIO_PULL3 0x2E

#define CFG_AI			 BIT_7
#define CFG_GPI_E_CFG	 BIT_6
#define CFG_OVR_FLOW_M	 BIT_5
#define CFG_INT_CFG		 BIT_4
#define CFG_OVR_FLOW_IEN BIT_3
#define CFG_K_LCK_IEN	 BIT_2
#define CFG_GPI_IEN		 BIT_1
#define CFG_KE_IEN		 BIT_0

#define INT_STAT_CAD_INT	  BIT_4
#define INT_STAT_OVR_FLOW_INT BIT_3
#define INT_STAT_K_LCK_INT	  BIT_2
#define INT_STAT_GPI_INT	  BIT_1
#define INT_STAT_K_INT		  BIT_0

#define tca8418_getReg(reg) i2c_readByte(TCA8418_ADDR, reg)

bool KeyDown[KEY_CNT];
const uint8_t Key2HID[17] = { 
	0x53, // Lock
	0x54, // /
	0x55, // *
	0x56, // -
	0x5F, // 7
	0x60, // 8
	0x61, // 9
	0x5C, // 4
	0x22, // 5
	0x5E, // 6
	0x57, // +
	0x59, // 1
	0x5A, // 2
	0x5B, // 3
	0x58, // .
	0x62, // 0
	0x63  // Enter
};           

//static uint8_t tca8418_getReg(uint8_t reg) {
//    uint8_t dat;
//	;
//    NRF_LOG_INFO("%x : %x",reg,dat);
//    return dat;
//}

static void tca8418_setReg(uint8_t reg, uint8_t val) {
    i2c_writeByte(TCA8418_ADDR, reg, val);
}

void tca8418_logReg(uint8_t reg) {
	NRF_LOG_INFO("%x : %x", reg, tca8418_getReg(reg));
}

void tca8418_logAll(void) {
	uint16_t i;
	for (i = 1; i <= 0x2e; ++i) {
		tca8418_logReg(i);
	}
}

bool tca8418_check(void) {
    tca8418_setReg(REG_CFG, 6);
    return tca8418_getReg(REG_CFG) != 6;
}

void tca8418_keyScan(void) {
	uint8_t col, row;
	row = tca8418_getReg(REG_GPIO_INT_STAT1);
	col = tca8418_getReg(REG_GPIO_INT_STAT2);
	KeyDown[5]  = row & BIT_0;
	KeyDown[8]  = row & BIT_1;
	KeyDown[0]  = row & BIT_2;
	KeyDown[12] = row & BIT_3;
	KeyDown[4]  = row & BIT_4;
	KeyDown[7]  = row & BIT_5;
	KeyDown[11] = row & BIT_6;
	KeyDown[15] = row & BIT_7;
	KeyDown[13] = col & BIT_0;
	KeyDown[9]  = col & BIT_1;
	KeyDown[16] = col & BIT_2;
	KeyDown[14] = col & BIT_3;
	KeyDown[10] = col & BIT_4;
	KeyDown[6]  = col & BIT_5;
	KeyDown[1]  = col & BIT_6;
	KeyDown[3]  = col & BIT_7;
}

void tca8418_clearInterrupts(void) {
//	while (tca8418_getReg(REG_KEY_EVENT_A)) {
//	}
	tca8418_getReg(REG_GPIO_INT_STAT1);
	tca8418_getReg(REG_GPIO_INT_STAT2);
	tca8418_getReg(REG_GPIO_INT_STAT3);
	tca8418_setReg(REG_INT_STAT, 0x1F);
}

void reg_init(void) {
	tca8418_setReg(REG_CFG, CFG_GPI_IEN | CFG_INT_CFG); //
	tca8418_setReg(REG_GPIO_INT_EN1, 0xFF);
	tca8418_setReg(REG_GPIO_INT_EN2, 0xFF);
//	tca8418_setReg(REG_GPI_EM1, 0xFF);
//	tca8418_setReg(REG_GPI_EM2, 0xFF);
}

void tca8418_init(void) {
	while (tca8418_check()) {
    }
	reg_init();
	tca8418_clearInterrupts();
	NRF_LOG_INFO("8418ready");
}

void keyboard_clearKeyDown(void) {
    uint8_t i;
	for (i = 0; i < 17; ++i)
		KeyDown[i] = false;
}

void keyboard_send(void);

void OnIntTrigger(void *p_event_data, uint16_t event_size) {
    tca8418_keyScan();
	keyboard_send();
    rgb_keyboardLoad();
    
//    tca8418_logReg(0x03);
//    tca8418_logReg(0x04);
//    tca8418_logReg(0x04);
//    tca8418_logReg(0x04);
	tca8418_clearInterrupts();
}

void OnIntClear(void *p_event_data, uint16_t event_size) {
    tca8418_keyScan();
}

void OnKey2Push(void *p_event_data, uint16_t event_size) {
    KeyDown[2] = true;
    keyboard_send();
    rgb_keyboardLoad();
    
    tca8418_logReg(REG_INT_STAT);
	tca8418_clearInterrupts();
}

void OnLeftKeyPush(void *p_event_data, uint16_t event_size) {
    KeyDown[17] = true;
    keyboard_send();
	tca8418_clearInterrupts();
}

void OnLeftKeyRelease(void *p_event_data, uint16_t event_size) {
    KeyDown[17] = false;
    keyboard_send();
}

void OnRightKeyPush(void *p_event_data, uint16_t event_size) {
    KeyDown[18] = true;
    keyboard_send();
}

void OnRightKeyRelease(void *p_event_data, uint16_t event_size) {
    KeyDown[18] = false;
    keyboard_send();
}

/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void keys_eventHandler(bsp_event_t event) {
    switch (event) {
        case BSP_EVENT_KEY_0: // key*
            app_sched_event_put(NULL, 0, OnKey2Push);
            break;
        case BSP_EVENT_KEY_1: // right
            app_sched_event_put(NULL, 0, OnRightKeyPush);
            break;
        case BSP_EVENT_KEY_2: // left
            app_sched_event_put(NULL, 0, OnLeftKeyPush);
            break;
        case BSP_EVENT_KEY_3: // interrupt
            app_sched_event_put(NULL, 0, OnIntTrigger);
            break;
        case BSP_EVENT_KEY_4:
            KeyDown[2] = false;
            break;
        case BSP_EVENT_KEY_5:
            app_sched_event_put(NULL, 0, OnIntClear);
            break;
        case BSP_EVENT_KEY_6: 
            app_sched_event_put(NULL, 0, OnLeftKeyRelease);
            break;
        case BSP_EVENT_KEY_7:
            app_sched_event_put(NULL, 0, OnRightKeyRelease);
            break;
        default: break;
    }
}
