/****************************** (C) COPYRIGHT 2013 YQDZ **************************
* 文  件  名      : TM1638CMD.H
* 作      者      : YQDZ
* 淘  宝  店      : http://zyqmcu.taobao.com
* 版      本      : V1.1
* 日      期      : 2013/01/17
* 主  控  MCU     : STC90C516RD+
* 主      频      : 11.0592MHz
* 编  译  器      : Keil C51 V3.8
* 描      述      : TM1638数码管芯片命令集.
*********************************************************************************/
/******************************** 数据命令设置 **********************************/
#define     ADDRESS_INCREASE_MODE       0X40                                            /* 地址自动增加模式 */
#define     READ_KEY                    0X42                                            /* 读键值 */
#define     ADDRESS_FIX_MODE            0X44                                            /* 固定地址模式 */

/******************************** 地址命令设置 **********************************/
#define     TM1638_DIG0                 0XC0                                            /* 数码管位0显示,需另加8位数据 */
#define     TM1638_DIG1                 0XC2                                            /* 数码管位1显示,需另加8位数据 */
#define     TM1638_DIG2                 0XC4                                            /* 数码管位2显示,需另加8位数据 */
#define     TM1638_DIG3                 0XC6                                            /* 数码管位3显示,需另加8位数据 */
#define     TM1638_DIG4                 0XC8                                            /* 数码管位4显示,需另加8位数据 */
#define     TM1638_DIG5                 0XCA                                            /* 数码管位5显示,需另加8位数据 */
#define     TM1638_DIG6                 0XCC                                            /* 数码管位6显示,需另加8位数据 */
#define     TM1638_DIG7                 0XCE                                            /* 数码管位7显示,需另加8位数据 */

/******************************** 显示控制设置 **********************************/
#define     DISPLAY_OFF                 0X80                                            /* 显示关 */
#define     DISPLAY_ON                  0X88                                            /* 显示开 */

#define     SET_PLUS_WIDTH1             0X88                                            /* 设置脉冲宽度为 1/16 */
#define     SET_PLUS_WIDTH2             0X89                                            /* 设置脉冲宽度为 2/16 */
#define     SET_PLUS_WIDTH4             0X8A                                            /* 设置脉冲宽度为 4/16 */
#define     SET_PLUS_WIDTH10            0X8B                                            /* 设置脉冲宽度为 10/16 */
#define     SET_PLUS_WIDTH11            0X8C                                            /* 设置脉冲宽度为 11/16 */
#define     SET_PLUS_WIDTH12            0X8D                                            /* 设置脉冲宽度为 12/16 */
#define     SET_PLUS_WIDTH13            0X8E                                            /* 设置脉冲宽度为 13/16 */
#define     SET_PLUS_WIDTH14            0X8F                                            /* 设置脉冲宽度为 14/16 */
