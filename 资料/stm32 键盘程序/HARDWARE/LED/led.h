# ifndef __LED_H
# define __LED_H
# include "sys.h"					//LED端口定义
# define PA8 PAout(8)				//振子 高导通
# define L1 PAout(13)				//L1 低导通
# define LA PAout(14)				//LA 低导通
# define PB0 PBout(0)				//列一 1 Q A 5 低导通
# define PB1 PBout(1)				//列二 2 W S Z 低导通
# define PB2 PBout(2)				//列三 3 E D X 低导通
# define PB3 PBout(3)				//列四 4 R F SP 低导通
# define PB4 PBout(4)				//横四 5 Z X SP 高导通
# define PB5 PBout(5)				//横三 A S D F 高导通
# define PB6 PBout(6)				//横二 Q W E R 高导通
# define PB7 PBout(7)				//横一 1 2 3 4 高导通
void LED_Init(void);
void LED_Shine(u8 led);
# endif 
