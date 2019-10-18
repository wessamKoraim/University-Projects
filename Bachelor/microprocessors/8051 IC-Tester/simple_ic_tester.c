#include <reg52.h>
#include "header.h"
#include "simple_ic_tester.h"

void SetupSerial();
void SendChar(unsigned char ch);

void main(void)
{
	unsigned int x;
	SetupSerial();
	SendChar('I'); SendChar('C'); SendChar(' '); SendChar('T'); SendChar('E'); SendChar('S'); SendChar('T'); SendChar('E');
	SendChar('R'); SendChar('\n');
	Test_switch = 1;
  initial:
	P1 = 0xFF; P2 = 0xFF;
	goto begin;
	NOT:
	P1 = 0xEA; P2 = 0xEA;
	goto backnot;
	NAND:
	P1 = 0xDB; P2 = 0xF6;
	goto backnand;
	OR:
	P1 = 0xDB; P2 = 0xF6;
	goto backor;
	AND:
	P1 = 0xDB; P2 = 0xF6;
	goto backand;
	XOR:
	P1 = 0xDB; P2 = 0xF6;
	goto backxor;
	AND3IN:
	P1 = 0xCF; P2 = 0xEE;
	goto backand3in;
	begin:
	while(1)
	{
		if(Test_switch == 0)
		{
			t_delay(20000);
			goto NOT;
			backnot:
			P1 = 0xFF; P2 = 0xFF;
			x = test_7404();
			if(x == 1)
			{ SendChar('7'); SendChar('4'); SendChar('0'); SendChar('4'); SendChar('\n'); goto initial; }
			t_delay(20000);
			goto NAND;
			backnand:
			P1 = 0xFF; P2 = 0xFF;
			x = test_7400();
			if(x == 1)
			{ SendChar('7'); SendChar('4'); SendChar('0'); SendChar('0'); SendChar('\n'); goto initial; }
			t_delay(20000);
			goto OR;
			backor:
			P1 = 0xFF; P2 = 0xFF;
			x = test_7432();
			if(x == 1)
			{ SendChar('7'); SendChar('4'); SendChar('3'); SendChar('2'); SendChar('\n'); goto initial; }
			t_delay(20000);
			goto AND;
			backand:
			P1 = 0xFF; P2 = 0xFF;
			x = test_7408();
			if(x == 1)
			{ SendChar('7'); SendChar('4'); SendChar('0'); SendChar('8'); SendChar('\n'); goto initial; }
			t_delay(20000);
			goto XOR;
			backxor:
			P1 = 0xFF; P2 = 0xFF;
			x = test_7486();
			if(x == 1)
			{ SendChar('7'); SendChar('4'); SendChar('8'); SendChar('6'); SendChar('\n'); goto initial; }
			t_delay(20000);
			goto AND3IN;
			backand3in:
			P1 = 0xFF; P2 = 0xFF;
			x = test_7411();
			if(x == 1)
			{ SendChar('7'); SendChar('4'); SendChar('1'); SendChar('1'); SendChar('\n'); goto initial; }
			if(x == 0)
			{ SendChar('U'); SendChar('n'); SendChar('d'); SendChar('e'); SendChar('f'); SendChar('i'); SendChar('n');
			  SendChar('e'); SendChar('d'); SendChar('\n'); goto initial; }
		}
	}
}

void SetupSerial()
{
    SCON  = 0x50;		        		
    TMOD |= 0x20;               
  	TH1   = 250;                
    TR1   = 1;                             
}

void SendChar(unsigned char ch)
{
    SBUF = ch;
		while(TI==0);
		TI = 0;
}

unsigned int test_7404()
{
	//P10 --> P11
	P10=0;
	if(P11==0)
	{ return 0; }
	P10=1;
	if(P11==1)
	{ return 0; }
	
	//P12 --> P13
	P12=0;
	if(P13==0)
	{ return 0; }
	P12=1;
	if(P13==1)
	{ return 0; }
	
	//P14 --> P15
	P14=0;
	if(P15==0)
	{ return 0; }
	P14=1;
	if(P15==1)
	{ return 0; }
	
	//P21 --> P20
	P21=0;
	if(P20==0)
	{ return 0; }
	P21=1;
	if(P20==1)
	{ return 0; }

	//P23 --> P22
	P23=0;
	if(P22==0)
	{ return 0; }
	P23=1;
	if(P22==1)
	{ return 0; }

	//P25 --> P24
	P25=0;
	if(P24==0)
	{ return 0; }
	P25=1;
	if(P24==1)
	{ return 0; }

	return 1;	
}

unsigned int test_7400()
{
	//(P10,P11) --> P12
	P10=0; P11=0;
	if(P12==0)
	{ return 0; }
	P10=0; P11=1;
	if(P12==0)
	{ return 0; }
	P10=1; P11=0;
	if(P12==0)
	{ return 0; }
	P10=1; P11=1;
	if(P12==1)
	{ return 0; }
	
	//(P13,P14) --> P15
	P13=0; P14=0;
	if(P15==0)
	{ return 0; }
	P13=0; P14=1;
	if(P15==0)
	{ return 0; }
	P13=1; P14=0;
	if(P15==0)
	{ return 0; }
	P13=1; P14=1;
	if(P15==1)
	{ return 0; }
	
	//(P22,P21) --> P20
	P22=0; P21=0;
	if(P20==0)
	{ return 0; }
	P22=0; P21=1;
	if(P20==0)
	{ return 0; }
	P22=1; P21=0;
	if(P20==0)
	{ return 0; }
	P22=1; P21=1;
	if(P20==1)
	{ return 0; }
	
	//(P25,P24) --> P23
	P25=0; P24=0;
	if(P23==0)
	{ return 0; }
	P25=0; P24=1;
	if(P23==0)
	{ return 0; }
	P25=1; P24=0;
	if(P23==0)
	{ return 0; }
	P25=1; P24=1;
	if(P23==1)
	{ return 0; }
	
	return 1;
}

unsigned int test_7432()
{
	//(P10,P11) --> P12
	P10=0; P11=0;
	if(P12==1)
	{ return 0; }
	P10=0; P11=1;
	if(P12==0)
	{ return 0; }
	P10=1; P11=0;
	if(P12==0)
	{ return 0; }
	P10=1; P11=1;
	if(P12==0)
	{ return 0; }
	
	//(P13,P14) --> P15
	P13=0; P14=0;
	if(P15==1)
	{ return 0; }
	P13=0; P14=1;
	if(P15==0)
	{ return 0; }
	P13=1; P14=0;
	if(P15==0)
	{ return 0; }
	P13=1; P14=1;
	if(P15==0)
	{ return 0; }
	
	//(P22,P21) --> P20
	P22=0; P21=0;
	if(P20==1)
	{ return 0; }
	P22=0; P21=1;
	if(P20==0)
	{ return 0; }
	P22=1; P21=0;
	if(P20==0)
	{ return 0; }
	P22=1; P21=1;
	if(P20==0)
	{ return 0; }
	
	//(P25,P24) --> P23
	P25=0; P24=0;
	if(P23==1)
	{ return 0; }
	P25=0; P24=1;
	if(P23==0)
	{ return 0; }
	P25=1; P24=0;
	if(P23==0)
	{ return 0; }
	P25=1; P24=1;
	if(P23==0)
	{ return 0; }
	
	return 1;
}

unsigned int test_7408()
{
	//(P10,P11) --> P12
	P10=0; P11=0;
	if(P12==1)
	{ return 0; }
	P10=0; P11=1;
	if(P12==1)
	{ return 0; }
	P10=1; P11=0;
	if(P12==1)
	{ return 0; }
	P10=1; P11=1;
	if(P12==0)
	{ return 0; }
	
	//(P13,P14) --> P15
	P13=0; P14=0;
	if(P15==1)
	{ return 0; }
	P13=0; P14=1;
	if(P15==1)
	{ return 0; }
	P13=1; P14=0;
	if(P15==1)
	{ return 0; }
	P13=1; P14=1;
	if(P15==0)
	{ return 0; }
	
	//(P22,P21) --> P20
	P22=0; P21=0;
	if(P20==1)
	{ return 0; }
	P22=0; P21=1;
	if(P20==1)
	{ return 0; }
	P22=1; P21=0;
	if(P20==1)
	{ return 0; }
	P22=1; P21=1;
	if(P20==0)
	{ return 0; }
	
	//(P25,P24) --> P23
	P25=0; P24=0;
	if(P23==1)
	{ return 0; }
	P25=0; P24=1;
	if(P23==1)
	{ return 0; }
	P25=1; P24=0;
	if(P23==1)
	{ return 0; }
	P25=1; P24=1;
	if(P23==0)
	{ return 0; }
	
	return 1;
}

unsigned int test_7486()
{
	//(P10,P11) --> P12
	P10=0; P11=0;
	if(P12==1)
	{ return 0; }
	P10=0; P11=1;
	if(P12==0)
	{ return 0; }
	P10=1; P11=0;
	if(P12==0)
	{ return 0; }
	P10=1; P11=1;
	if(P12==1)
	{ return 0; }
	
	//(P13,P14) --> P15
	P13=0; P14=0;
	if(P15==1)
	{ return 0; }
	P13=0; P14=1;
	if(P15==0)
	{ return 0; }
	P13=1; P14=0;
	if(P15==0)
	{ return 0; }
	P13=1; P14=1;
	if(P15==1)
	{ return 0; }
	
	//(P22,P21) --> P20
	P22=0; P21=0;
	if(P20==1)
	{ return 0; }
	P22=0; P21=1;
	if(P20==0)
	{ return 0; }
	P22=1; P21=0;
	if(P20==0)
	{ return 0; }
	P22=1; P21=1;
	if(P20==1)
	{ return 0; }
	
	//(P25,P24) --> P23
	P25=0; P24=0;
	if(P23==1)
	{ return 0; }
	P25=0; P24=1;
	if(P23==0)
	{ return 0; }
	P25=1; P24=0;
	if(P23==0)
	{ return 0; }
	P25=1; P24=1;
	if(P23==1)
	{ return 0; }
	
	return 1;
}

unsigned int test_7411()
{
	//(P10,P11,P25) --> P24
	P10=0; P11=0; P25=0;
	if(P24==1)
	{ return 0; }
	P10=0; P11=0; P25=1;
	if(P24==1)
	{ return 0; }
	P10=0; P11=1; P25=0;
	if(P24==1)
	{ return 0; }
	P10=0; P11=1; P25=1;
	if(P24==1)
	{ return 0; }
	P10=1; P11=0; P25=0;
	if(P24==1)
	{ return 0; }
	P10=1; P11=0; P25=1;
	if(P24==1)
	{ return 0; }
	P10=1; P11=1; P25=0;
	if(P24==1)
	{ return 0; }
	P10=1; P11=1; P25=1;
	if(P24==0)
	{ return 0; }
	
	//(P12,P13,P14) --> P15
	P12=0; P13=0; P14=0;
	if(P15==1)
	{ return 0; }
	P12=0; P13=0; P14=1;
	if(P15==1)
	{ return 0; }
	P12=0; P13=1; P14=0;
	if(P15==1)
	{ return 0; }
	P12=0; P13=1; P14=1;
	if(P15==1)
	{ return 0; }
	P12=1; P13=0; P14=0;
	if(P15==1)
	{ return 0; }
	P12=1; P13=0; P14=1;
	if(P15==1)
	{ return 0; }
	P12=1; P13=1; P14=0;
	if(P15==1)
	{ return 0; }
	P12=1; P13=1; P14=1;
	if(P15==0)
	{ return 0; }
	
	//(P23,P22,P21) --> P20
	P23=0; P22=0; P21=0;
	if(P20==1)
	{ return 0; }
	P23=0; P22=0; P21=1;
	if(P20==1)
	{ return 0; }
	P23=0; P22=1; P21=0;
	if(P20==1)
	{ return 0; }
	P23=0; P22=1; P21=1;
	if(P20==1)
	{ return 0; }
	P23=1; P22=0; P21=0;
	if(P20==1)
	{ return 0; }
	P23=1; P22=0; P21=1;
	if(P20==1)
	{ return 0; }
	P23=1; P22=1; P21=0;
	if(P20==1)
	{ return 0; }
	P23=1; P22=1; P21=1;
	if(P20==0)
	{ return 0; }
	
	return 1;
}	