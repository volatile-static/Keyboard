# ifndef __KEY_H
# define __KEY_H
# include "sys.h"
# define h1 PAin(7)
# define h2 PAin(6)
# define h3 PAin(5)
# define h4 PAin(4)
# define K1_PRES 0X0001
# define K2_PRES 0X0002
# define K3_PRES 0X0004
# define K4_PRES 0X0008
# define KQ_PRES 0X0010
# define KW_PRES 0X0020
# define KE_PRES 0X0040
# define KR_PRES 0X0080
# define KA_PRES 0X0100
# define KS_PRES 0X0200
# define KD_PRES 0X0400
# define KF_PRES 0X0800
# define K5_PRES 0X1000
# define KZ_PRES 0X2000
# define KX_PRES 0X4000
# define KSP_PRES 0X8000
void Key_Init(void);
u16 Key_Scan(void);
#endif

