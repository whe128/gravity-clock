#ifndef __STC8G_UART_H
#define __STC8G_UART_H

#include "STC8G_Register.h"
#include "sys.h"
#include "config.h"



#define	UART_8bit_BRTx	(1<<6)	// 8-bit data, variable baud rate

// Interrupt priority
#define	PSH		0x10

#define	Uart1RxEnable(n)	(n==0?(REN = 0):(REN = 1))	/* Enable/disable UART1 reception */
#define	Uart1Interrupt(n)	(n==0?(ES = 0):(ES = 1))	/* Enable/disable UART1 interrupt */
// UART1 interrupt priority control

#define Uart1Priority(n) do{if(n == 0) IPH &= ~PSH, PS = 0; \
																if(n == 1) IPH &= ~PSH, PS = 1; \
																if(n == 2) IPH |= PSH, PS = 0; \
																if(n == 3) IPH |= PSH, PS = 1; \
															}while(0)

#ifdef DEBUG
extern u8 TXBusy;
void Uart1Configuration();
#endif


#endif
