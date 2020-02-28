#include <STC12C5A60S2.H>

void ConfigUART(unsigned int baud);//Set Baud Rate

void main()
{
	ConfigUART(9600);//The Baud Rate is 9600

	while(1)
	{
		while(!RI);
		RI = 0;//Reception Stop
		SBUF = SBUF + 1;
		while(!TI);
		TI = 0;	//	Send Stop
	}
}

void ConfigUART(unsigned int baud)
{
	SCON = 0x50;
	TMOD |= 0x20;//Work Mode 2
	TH1 = 256 - (33177600/12/32)/baud;
	TL1 = TH1;
	ET1 = 0;//Timer 1 Interrupt Shut
	TR1 = 1;//Timer 1 Work
}