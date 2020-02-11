#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define BIT_ADDR(addr, bitnum) *(volatile unsigned long *)(BITBAND(addr, bitnum))

#define PAout(n) BIT_ADDR(0x4001080C, n)
#define PAin(n)  BIT_ADDR(0x40010808, n)
#define PBout(n) BIT_ADDR(0x40010C0C, n)
#define PBin(n)  BIT_ADDR(0x40010C08, n)

#define LED_Caps PBout(0)
#define LED_Num PBout(10)
#define LED_Scroll PBout(11)
#define LED0 PBout(8)
#define LED1 PBout(9)
#define LED2 PAout(15)

#define CLK PBout(13)
#define STB PBout(12)
#define DO  PBout(15)
#define DI  PBin(15)
