#include <STC12C5A60S2.H>
sbit key=P2^7;
sbit ENLED = P1^1;
void Delay();//Delay 10ms
unsigned char cnt=0;
unsigned char  LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};//Character Encoded Array

void main()
{
	bit backup=1;//Falg of the Key
	P2=0xF8;
	ENLED=0;
	P0=LedChar[cnt];
	while(1)
	{
	if(key!=backup)//Judge Key State
	{
		Delay();
		backup=key;
		if(backup==0)
		{
			cnt++;
			if(cnt>=10)
			{
				cnt=0;
			}
			P0=LedChar[cnt];
		}
		
	}	
}
	
}
void Delay()		//@33.1776MHz 10ms
{
	unsigned char i, j, k;

	//_nop_();
	//_nop_();
	i = 2;
	j = 67;
	k = 183;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
