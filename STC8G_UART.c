
#include "STC8G_UART.h"


#ifdef DEBUG

u8	TXBusy;

void Uart1Configuration()
{
	TXBusy=0;

	SCON = (SCON & 0x3f) | UART_8bit_BRTx;					//mode setting
	TR1 = 0;
	AUXR &= ~0x01;					//S1 BRT Use Timer1;
	TMOD &= ~(1<<6);				//Timer1 set As Timer
	TMOD &= ~0x30;					//Timer1_16bitAutoReload;
	AUXR |=  (1<<6);				//Timer1 set as 1T mode
	TH1 = (65536UL- (MAIN_Fosc / 4) / 115200ul)/256;		//115200 bps
	TL1 = (65536UL- (MAIN_Fosc / 4) / 115200ul)%256;
	ET1 = 0;						//close the timer1 interrupt
	TMOD &= ~0x40;					//for timer1
	INTCLKO &= ~0x02;				//not output clock on T1 pin
	TR1  = 1;
	Uart1RxEnable(ENABLE);			//uart1 receive enable
}

//printf function
char putchar(char c)
{
	SBUF = c;
	TXBusy = 1;		//flag busy
	while(TXBusy);
	return c;
}

/*
void PrintString1(u8 *puts)
{
    for (; *puts != 0;	puts++)   //meet the end sign of string
    {

		SBUF = *puts;
		TXBusy = 1;		//flag busy
		while(TXBusy);
    }
}
*/
#endif
