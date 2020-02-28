#include <STC12C5A60S2.H>
sbit ENLED=P1^1;
unsigned int t=0;
unsigned char i=0;
unsigned char led[]={
	0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F
};//LED State Table
	
void main()
{
	ENLED=0;
	P2=0xFE;
	EA=1;
	TMOD=0x01;//Work Mode 1
	TR0=1;
	TH0=0x93;
	TL0=0xFF;//10ms
	ET0=1;//Timer0 Interrupt Work
	while (1);
	
}
void timer() interrupt 1
{
	TH0=0x93;
	TL0=0xFF;
	t++;
	if (t>=50)//0.5s
	{
		t=0;
		P0=led[i];
		i++;//LEDs Go on And Off Alternately   
		if(i>=8)
		{
			i=0;
		}
	}
}
