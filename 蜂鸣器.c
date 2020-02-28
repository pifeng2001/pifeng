#include <STC12C5A60S2.H>
sbit BUZZ=P1^3;
unsigned char T0RH;
unsigned char T0RL;
void BUZZ1(unsigned int HZ);//High Potential (frequency)
void BUZZ0();//Low Potential
void Delay1000ms();

void main()
{
	EA=1;
	TMOD =0x01;//Work Mode 1
	while(1)
	{
		BUZZ1(5000);
		Delay1000ms();
		BUZZ0();
		Delay1000ms();
		BUZZ1(2000);
		Delay1000ms();
		BUZZ0();
		Delay1000ms();
	}
}

void BUZZ1(unsigned int HZ)//High Potential (frequency)
{
	unsigned int reload;
	reload=65535-(33177600/12)/(HZ*2);
	T0RH=(unsigned char)(reload>>8);//Four High
	T0RL=(unsigned char)reload;//Four Low 
	TH0=0xFF;
	TL0=0xFE;//Into Interrupt Fastly
	TR0=1;//Timer0 Work
	ET0=1;//Timer0 Interrupt Work
}
void BUZZ0()////Low Potential
{
	ET0=0; //Timer0 Stop
	TR0=0; //Timer0 Interrupt Shut
	
}
void interrupttimer0 () interrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	BUZZ=~BUZZ;
}
void Delay1000ms()		//@33.1776MHz
{
	unsigned char i, j, k;

	//_nop_();
	//_nop_();
	i = 127;
	j = 18;
	k = 107;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

