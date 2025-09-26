#ifndef __STC8G_TIMER_H
#define __STC8G_TIMER_H

#include "STC8G_Register.h"


//Time configuration
#define	Timer0						0
#define	Timer2						2
#define	TIM_16BitAutoReload			0
#define	TIM_16Bit					1
#define	TIM_8BitAutoReload			2
#define	TIM_16BitAutoReloadNoMask	3
#define	TIM_T1Stop					3

#define	TIM_CLOCK_12T				0
#define	TIM_CLOCK_1T				1
#define	TIM_CLOCK_Ext				2

#define	PT0H						0x02

//timer0
#define		Timer0_Status()					(TCON & 0x10) >> 4
#define 	Timer0_Run()					TR0 = 1				/* Start Timer 0 count */
#define 	Timer0_Stop()	 				TR0 = 0				/* Stop Timer 0 count */
#define		Timer0_SetMode(mode)			TMOD = (TMOD & ~0x03) | (mode)
#define 	Timer0_ClkSelect(n)				do{if(n == 0) AUXR &= ~(1<<7), TMOD &= ~(1<<2); \
																if(n == 1) AUXR |= (1<<7), TMOD &= ~(1<<2); \
																if(n == 2) TMOD |= (1<<2); \
																}while(0)
#define		Timer0_ClkOutput(n)				INTCLKO = (INTCLKO & ~0x01) | (n)	/* T0 overflow pulse output enable on T0 pin */
#define		Timer0_Load(n)					TH0 = (n) / 256,	TL0 = (n) % 256
#define 	Timer0_Interrupt(n)				(n==0?(ET0 = 0):(ET0 = 1))				/* Timer0 interrupt enable */
#define 	Timer0_Priority(n)				do{if(n == 0) IPH &= ~PT0H, PT0 = 0; \
																if(n == 1) IPH &= ~PT0H, PT0 = 1; \
																if(n == 2) IPH |= PT0H, PT0 = 0; \
																if(n == 3) IPH |= PT0H, PT0 = 1; \
															}while(0)

//timer2
#define		Timer2_Status()					(AUXR & 0x10) >> 4
#define 	Timer2_Run()					 AUXR |= (1 << 4)			/* Start Timer 2 */
#define 	Timer2_Stop()	 				 AUXR &= ~(1<<4)			/* Stop Timer 2 count */
#define 	Timer2_ClkSelect(n)				{if(n == 0) AUXR &= ~(1<<2), AUXR &= ~(1<<3); \
																if(n == 1) AUXR |= (1<<2), AUXR &= ~(1<<3); \
																if(n == 2) AUXR |= (1<<3); \
																}while(0)
#define		Timer2_ClkOutput(n)				INTCLKO = (INTCLKO & ~0x04) | (n << 2)	/* T2 overflow pulse output enable on T2 pin */
#define		Timer2_Load(n)					T2H = (n) / 256,	T2L = (n) % 256
#define		Timer2_Ps(n)					TM2PS = n
#define 	Timer2_Interrupt(n)				IE2 = (IE2 & ~0x04) | (n << 2)			/* Timer2 interrupt enable */


#endif
