#include "led.h"
#include <altera_avalon_timer_regs.h>
#include <math.h>
#include <priv/alt_legacy_irq.h>
#include <stdbool.h>
#include <stdlib.h>
#include <system.h>

static const alt_u8 rgb_map[6][7] = {
    {74, 75, 76, 107, 107, 107, 107}, // 0
    {77, 78, 79, 80, 81, 82, 83},     // 1
    {84, 85, 86, 87, 88, 89, 90},     // 2
    {107, 107, 107, 91, 92, 93, 107}, // 3
    {107, 94, 107, 95, 96, 97, 107},  // 4
    {98, 99, 100, 101, 107, 102, 103} // 5
};
static const alt_u8 ice_boundary[7] = {0, 12, 26, 40, 53, 65, 72};

static rgb_t COLOR_DAT[COLOR_DAT_SIZE];
static bool ice_flag, rgb_flag;

static void GenerateColorData(void) {
    unsigned char i, j;
    for (i = 0; i < 6; ++i)
        for (j = 0; j <= MAX_BRIGHTNESS; ++j)
            switch (i) {
            case 0:
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.green = j;
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.red = MAX_BRIGHTNESS;
                break;
            case 1:
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.red =
                    MAX_BRIGHTNESS - j;
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.green = MAX_BRIGHTNESS;
                break;
            case 2:
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.blue = j;
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.green = MAX_BRIGHTNESS;
                break;
            case 3:
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.green =
                    MAX_BRIGHTNESS - j;
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.blue = MAX_BRIGHTNESS;
                break;
            case 4:
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.red = j;
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.blue = MAX_BRIGHTNESS;
                break;
            case 5:
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.blue =
                    MAX_BRIGHTNESS - j;
                COLOR_DAT[i * MAX_BRIGHTNESS + j].color.red = MAX_BRIGHTNESS;
                break;
            }
}

void Timer_isr(void *context) { //定时器中断服务函数
    static alt_u8 cnt;
    ++cnt;
    if (cnt % 100 == 0)
        ice_flag = true;
    if (cnt % 6 == 0)
        rgb_flag = true;
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0); //清状态寄存器
}

void led_init(void) {
    GenerateColorData();
    srand(233);
    alt_irq_register(TIMER_0_IRQ, 0, Timer_isr);      //注册中断函数
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0); //清状态标志
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,
                                     7); //启动定时器允许中断，连续计数
}

static ALT_INLINE void sched_rgb(void) {
    static unsigned short p;
    rgb_flag = false;
    --p;
    for (alt_u8 i = 0; i < 6; ++i)
        for (alt_u8 j = 0; j < 7; ++j)
            if (rgb_map[i][j] <= RGB_END)
                SET_LED(rgb_map[i][j],
                        COLOR_DAT[(p + 20 * j) % COLOR_DAT_SIZE].argb);
}

static ALT_INLINE void sched_ice(void) {
    static alt_8 pos[7] = {0, 6, 5, 4, 3, 2, 1}, len[7] = {0, 1, 2, 3, 4, 5, 6};
    ice_flag = false;
    for (alt_u8 i = 1; i <= 6; ++i) {
        ++pos[i];
        for (alt_u8 j = MAX(pos[i] - len[i], ice_boundary[i - 1] + 1);
             j < MIN(pos[i], ice_boundary[i]) + 1; ++j)
            SET_LED(j, 255.0f * (j - pos[i]) / len[i]);
        if (pos[i] - len[i] >= ice_boundary[i]) {
            len[i] = rand() % (ice_boundary[i] - ice_boundary[i - 1] - 5) + 3;
            pos[i] = ice_boundary[i - 1] - 10;
        }
    }
}

void led_sched(void) {
    if (ice_flag)
        sched_ice();
    if (rgb_flag)
        sched_rgb();
}
