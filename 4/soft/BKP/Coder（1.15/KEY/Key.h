# ifndef __KEY_H
# define __KEY_H
# include "sys.h"
# include "delay.h"
# define KEY0 PAin(4)
# define KEY1 PAin(5)
# define KEY2 PAin(6)
# define KEY3 PAin(7)

void KeyInit(void);
u8 KeyScan();
#endif

