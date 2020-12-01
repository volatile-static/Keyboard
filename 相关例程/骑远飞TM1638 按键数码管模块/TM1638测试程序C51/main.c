

#include	<REGX51.H>
#include	<tm1638.h>

unsigned char num[8];		//各个数码管显示的值

int main(void)
{
	unsigned char i;
	init_TM1638();	                           //初始化TM1638
	for(i=0;i<8;i++)
	Write_DATA(i<<1,tab[0]);	               //初始化寄存器
	
	while(1)
	{
		i=Read_key();                          //读按键值
		if(i<8)
		{
			num[i]++;
			while(Read_key()==i);		       //等待按键释放
			if(num[i]>15)
			num[i]=0;
			Write_DATA(i*2,tab[num[i]]);
			Write_allLED(1<<i);
		}
	}
}

