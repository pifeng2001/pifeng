#include <STC12C5A60S2.H>

#define DB  P0
bit GetTemp(int *temp);
bit flag=0;
bit StartDS();
sbit EN = P1^2;
sbit IO_18B20 = P3^5; 
sbit WR = P2^1;
sbit RS = P2^0;
unsigned char T0RH = 0;  
unsigned char T0RL = 0;  

unsigned char ToString(unsigned char *str ,int dat);
void IntLcd();
void ConfigTimer0(unsigned int ms);
void Cmd(unsigned char cmd);
void Ready();
void LcdShow(unsigned char x,unsigned char y,unsigned char *dat);
void XY(unsigned char x,unsigned char y);
void Delay10us(unsigned char j);
void Dat(unsigned char dat);
void WriteDS();
unsigned char  ReadDS();
void Delay2us()	;
void main()
{
	
	bit res;
	int temp;
	int intT;
	int decT;
	unsigned char len;
	unsigned char str[12];
	EA=1;
	ConfigTimer0(10);
	IntLcd();
	StartDS();
	while(1)
	{
		if(flag)
		{
			flag=0;
			res=GetTemp(&temp);
			if(res)
			{
				intT=temp>>4;
				decT=temp&0xF0;
				len=ToString(str,intT);
				str[len++]='.';
				decT = (decT*10) / 16; 
				str[len++]=decT+'0';
				while(len<6)
				{
					str[len++]=' ';
					
				}
				str[len]='\0';
				LcdShow(0,0,str);
			}
			else
			{
				LcdShow(0,0,"error!");
			}
			StartDS();
			}			
		}	
	}
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr1s = 0;
    
    TH0 = T0RH;  
    TL0 = T0RL;
    tmr1s++;
    if (tmr1s >= 100)  
    {
        tmr1s = 0;
        flag = 1;
    }
}
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  
    
    tmp = 33177600 / 12;      
    tmp = (tmp * ms) / 1000;  
    tmp = 65536 - tmp;     
    tmp = tmp + 12;           
    T0RH = (unsigned char)(tmp>>8);  
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   
    TMOD |= 0x01;   
    TH0 = T0RH;     
    TL0 = T0RL;
    ET0 = 1;        
    TR0 = 1;        
}

unsigned char ToString(unsigned char *str, int dat)
	{
		signed char i=0;
		unsigned char len=0;
		unsigned char buf[6];
		if(dat<0)
		{
			dat=-dat;
			*str++='-';
			len++;
			
		}
		do
		{
			buf[i++]=dat%10;
			dat%=10;
			
		}while(dat>0);
		len+=i;
		while(i-->0)
		{
			*str++=buf[i]+'\0';
		}
		*str='\0';
		return len;
	}
 bit GetAck()
{
	bit ack;
	EA=0;
	IO_18B20=0;
	Delay10us(50);
	IO_18B20=1;
	Delay10us(6);
	ack=IO_18B20;
	while(!IO_18B20)
	EA=1;
	return ack;
}
void WriteDS(unsigned char dat)
{
	unsigned char mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<1)
	{
		IO_18B20=0;
		Delay2us()	;
		if((mask&dat)==0)
			IO_18B20=0;
		else
			IO_18B20=1;
		Delay10us(6);
		IO_18B20=1;		
	}
	EA=1;
}
unsigned char ReadDS()
{
	unsigned char dat;
	unsigned char mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<1)
	{
		IO_18B20=0;
		Delay2us()	;
		IO_18B20=1;
		Delay2us()	;
		if(!IO_18B20)
			dat&=~mask;
		else
			dat|=mask;
		Delay10us(6);
	}
	EA=1;
	return dat;
}
bit StartDS()
{
	bit ack;
	ack=GetAck();
	if(ack==0)
	{
		WriteDS(0xCC);
		WriteDS(0x44);
	}
	return ~ack;
}
bit GetTemp(int *temp)
{
	bit ack;
	unsigned char LSB;
	unsigned char MSB;
	ack=GetAck();
	if(ack==0)
	{
		WriteDS(0xCC);
		WriteDS(0xBE);
		LSB=ReadDS();
		MSB=ReadDS();
		*temp=((int)MSB>>8)+LSB;
	}
	return~ack;
}
void IntLcd()
{
	Cmd(0x38);
	Cmd(0x0C);
	Cmd(0x06);
	Cmd(0x01);
}

void Cmd(unsigned char cmd)
{ 
	Ready();
	WR = 0;
	RS = 0;
	DB = cmd;
	EN = 1;
	EN = 0;
}
 void Ready()
 {
	 unsigned char db;
	 DB = 0xFF;
	 RS =0;
	 WR =1;
	 do
	 {
		 EN=1;
		 db = DB;
		 EN=0;
	 }while(db & 0x80);
	 
 }
 void LcdShow(unsigned char x,unsigned char y,unsigned char *dat)
 {
	 XY(x,y);
	 while(*dat!='\0')
	 {
		 Dat(*dat++);
	 }
 }
 void XY(unsigned char x,unsigned char y)
{
	unsigned char xy;
		if(y==0)
				xy=0x00+x;
		else
				xy=0x40+x;
		Cmd(xy | 0x80);
}
void Dat(unsigned char dat)
{
	Ready();
	RS = 1;
	WR = 0;
	DB = dat;
	EN=1;
	EN=0;
}

void Delay10us(unsigned char j)		//@33.1776MHz
{
	unsigned char i;
	while(j--)
	{
	//_nop_();
	//_nop_();
	i = 80;
	while (--i);
	}
}
void Delay2us()		//@33.1776MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}
