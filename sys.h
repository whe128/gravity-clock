#ifndef __SYS_H
#define __SYS_H

#include "STC8G_Register.h"
#include <intrins.h>

typedef unsigned char   u8;     //  8 bits
typedef unsigned int    u16;    // 16 bits
typedef unsigned long   u32;    // 32 bits
typedef unsigned char	bool;

#define	TRUE	1
#define	FALSE	0

#define ENABLE		1
#define DISABLE		0

#define	NULL		0

#define	Priority_0			0	// Interrupt priority level 0 (lowest)
#define	Priority_1			1	// Interrupt priority level 1 (low)
#define	Priority_2			2	// Interrupt priority level 2 (high)
#define	Priority_3			3	// Interrupt priority level 3 (highest)

// Enable extended SFR
#define	EAXSFR()			P_SW2 |= 0x80		/* MOVX A,@DPTR or MOVX @DPTR,A instructions access extended SFR (XSFR) */
#define EXIT_ENABLE()		EA= 1				/* Enable global interrupt */

// Note: not equal to 1
#define GET_BIT(data,n)		((data)&(0x01<<n))
#define SET_BIT(data,n,b)	(b==0?(data &= ~(0x01<<n)):(data |= (0x01<<n)))

void sysGpioConfig(void);
void sysTimerConfig(void);
void sysI2cConfig(void);
void sysUartConfig(void);
void sysDelayMs(unsigned char ms);
void sysSleepAlarm(u8 enable);
void sysSleepMode(void);
void sysOpenGpio(void);

#endif
