#include <REG51.H>
sbit ENLED=P1^1;
unsigned int t=0;
unsigned char i=0;
unsigned char led[]={
	0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F
};
	
void main()
{
	ENLED=0;
	P2=0xFE;
	EA=1;
	TMOD=0x01;
	TR0=1;
	TH0=0x93;
	TL0=0xFF;
	ET0=1;
	while (1);
	
}
void timer() interrupt 1
{
	TH0=0x93;
	TL0=0xFF;
	t++;
	if (t>=50)
	{
		t=0;
		P0=led[i];
		i++;
		if(i>=8)
		{
			i=0;
		}
	}
}
