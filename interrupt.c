#include "STC8G_Timer.h"
#include "STC8G_I2C.h"
#include "STC8G_UART.h"
#include "STC8G_INT.h"
#include "STC8G_Register.h"
#include "beep.h"
#include "control.h"
#include "config.h"

#ifdef DEBUG
#include  "stdio.h"
#endif

// Used for buzzer
void Timer0_Handler (void) interrupt TMR0_VECTOR		// Interrupt flag already cleared upon entry
{
	beepSound();
}
void Timer2Handler (void) interrupt TMR2_VECTOR			//Interrupt flag already cleared upon entry
{
	beepControl();
	controlbuttonTime2();
	controlScanTime2();
}

void INT2_Handler (void) interrupt INT2_VECTOR			//Interrupt flag already cleared upon entry
{
	#if 0
	if(ctrlValue.intFlag!=1 || ctrlValue.intFlag!=0)
	{
		ctrlValue.intFlag=0;
	}
	else
	{
		ctrlValue.intFlag=1-ctrlValue.intFlag;
	}
	#endif
}

void I2C_Handler() interrupt I2C_VECTOR
{
	// TODO: add user code here
}

#ifdef DEBUG
void UART1_Handler (void) interrupt UART1_VECTOR
{
	if(TI)
	{
		TI = 0;       // Clear interrupt flag
		TXBusy = 0;   // Using blocking send, directly clear busy flag
	}
}
#endif

