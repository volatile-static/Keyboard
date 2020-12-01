#include "delay.h"

void delay_init(void) {
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void delay_us(u32 nus) {
    SysTick->LOAD = nus * 9; // 时间加载
    SysTick->VAL = 0x00; // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    while ((SysTick->CTRL & 0x01) && !(SysTick->CTRL & (1 << 16))) {
    } // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00; // 清空计数器
}

void delay_ms(u32 nms) {
    while (nms--) {
        delay_us(1000);
    }
}
