#include <STC12C5A60S2.H>
sbit IR_INPUT = P3^2; 
sbit ENLED = P1^1;
bit irflag = 0;  //Infrared Flag
unsigned char ircode[4];//Cache region
unsigned char  LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};//Character Encoding 
void InitInfrared(void);// Infrared Initialize
void Delay1ms();
void main()
{
	EA=1;
	ENLED=0;
	InitInfrared();
	
	while(1)
	{
		if(irflag)//Judgment marker
		irflag=0;	
			P2=0X00;
		P0=LedChar[ircode[0]>>4];
			Delay1ms();
			P2=0xF9;
		P0=LedChar[ircode[0]&0x0F];//User Code
			Delay1ms();
			P2=0xFC;
		P0=LedChar[ircode[2]>>4];
			Delay1ms();
			P2=0xFD;
		P0=LedChar[ircode[2]&0x0F];//Key Code
			Delay1ms();
}	
	}
void InitInfrared()//Infrared Initialize
{	
	IR_INPUT = 1;
	TMOD &= 0X0F;
	TMOD |= 0x10;//Work Mode 1
	TR1 = 0;//Timer1 Shut
	ET0 = 0;//Timer0 Interrupt Shut
	IT1 = 1;//External Interrupt 1 Set Mode
	EX0 = 1;//External Interrupt 1 Work
}
unsigned int GetHighTime()//High Level Time
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(IR_INPUT)
	{
		if(TH1 > 0xC2)
		{
			break;
		}
	}
	TR1 = 0;

	return(TH1 * 256 + TL1);
}

unsigned int GetLowTime()//Low Level Time
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(!IR_INPUT)
	{
		if(TH1 > 0xC2)
		{
			break;
		}
	}
	TR1 = 0;

	return(TH1 * 256 + TL1);
}
void EXINT1_ISR() interrupt 0
{
	unsigned char i, j;
	unsigned int time;
	unsigned char byt;

	time = GetLowTime();
	if((time <23500) || (time > 26265))  
	{
		IE0 = 0;
		return;	
	}

	time = GetHighTime();
	if((time<11059) || (time > 13824))
	{
		IE0 = 0;
		return;
	}
	for(i=0; i<4; i++)
	{
		for(j=0; j<8; j++)
		{
			time = GetLowTime();
			if((time<940) ||(time >2157))
			{
				IE0 = 0;
				return;
			}
			time = GetHighTime();
			if((time>940) && (time <2157))
			{
				byt >>= 1;	
			}
			else if((time>4037) && (time<5253))
			{
				byt >>= 1;
				byt |= 0x80;
			}
			else
			{
				IE0 = 0;
				return;
			}
		}
		ircode[i] = byt;
	}
	
	IE0 = 0;
	irflag=1;
}
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

