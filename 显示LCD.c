#include <STC12C5A60S2.H>
#define DB  P0
sbit EN = P1^2;
sbit WR = P2^1;
sbit RS = P2^0;
void Int();//LCD Initalization
void Cmd(unsigned char cmd);//Write Cmd
void Ready();//LCD Ready?
void Show(unsigned char x,unsigned char y,unsigned char *dat);//Screen display
void XY(unsigned char x,unsigned char y);//Coordinates
void Delay10ms();
void Dat(unsigned char dat);//Write Data
void main()
{
	Int();
	Show(5,0,"Hello");//Show  the Char of ¡®Hello¡¯ on the First Line
	Show(5,1,"World");//Show  the Char of ¡®World¡¯ on the Secend Line
	while(1);
}
void Int()//LCD Initalization
{
	Cmd(0x38);
	Cmd(0x0C);
	Cmd(0x06);
	Cmd(0x01);
}

void Cmd(unsigned char cmd)//Write Cmd
{ 
	Ready();
	WR = 0;
	RS = 0;
	DB = cmd;
	Delay10ms();
	EN = 1;
	EN = 0;
}
 void Ready()//LCD Ready?
 {
	 unsigned char db;
	 DB = 0xFF;
	 RS =0;
	 WR =1;
	 do
	 {
		 EN=1;
		 db = DB;
		 Delay10ms();
		 EN=0;
	 }while(db & 0x80);
	 
 }
 void Show(unsigned char x,unsigned char y,unsigned char *dat)//Screen display
 {
	 XY(x,y);//
	 while(*dat!='\0')
	 {
		 Dat(*dat++);
	 }
 }
 void XY(unsigned char x,unsigned char y)//Coordinates(X,Y)
{
	unsigned char xy;
		if(y==0)//Determine Number of Rows
				xy=0x00+x;//Firsr Line
		else
				xy=0x40+x;//Secend Line
		Cmd(xy | 0x80);
}
void Dat(unsigned char dat)//Write Data
{
	Ready();
	RS = 1;
	WR = 0;
	DB = dat;
	Delay10ms();
	EN=1;
	EN=0;
}
void Delay10ms()		//@33.1776MHz 
{
	unsigned char i, j, k;

	//_nop_();
//	_nop_();
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

