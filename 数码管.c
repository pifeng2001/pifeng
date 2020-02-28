#include <STC12C5A60S2.H>
sbit ENLED=P1^1;
void Delay1ms()		//@33.1776MHz
{
	unsigned char i, j;

	//_nop_();
	//_nop_();
	i = 33;
	j = 66;
	do
	{
		while (--j);
	} while (--i);
}

unsigned char  LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90
	};//Character Encoding 

		void main()
		{while(1)
			{
			ENLED=0;
			P2=0X00;
		P0=LedChar[2];
			Delay1ms();
			P2=0xF9;
		P0=LedChar[0];
			Delay1ms();
			P2=0xFA;
		P0=LedChar[2];
			Delay1ms();
			P2=0xFB;
		P0=LedChar[0];
			Delay1ms();
			P2=0xFC;
		P0=LedChar[0];
			Delay1ms();
			P2=0xFD;
		P0=LedChar[1];
			Delay1ms();
			}
		}
		
		
	