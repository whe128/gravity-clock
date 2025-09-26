#include "sys.h"
#include "config.h"
#include "STC8G_GPIO.h"
#include "STC8G_Timer.h"
#include "STC8G_I2C.h"
#include "STC8G_UART.h"
#include "STC8G_INT.h"

void sysGpioConfig(void)
{
	GPIO_PULL_UP_P3(PIN_All);		//I2C(2,3) ballRotation(4,5) ballSwitch(6) beep(7)
	GPIO_PULL_UP_P1(PIN0|PIN1);		//button
	GPIO_PULL_UP_P1(PIN6|PIN7);		//button

	GPIO_DI_ENABLE_P0(PIN_All);
	GPIO_DI_ENABLE_P1(PIN_All);
	GPIO_DI_ENABLE_P2(PIN_All);
	GPIO_DI_ENABLE_P3(PIN_All);
	GPIO_DI_ENABLE_P4(PIN_All);
	GPIO_DI_ENABLE_P5(PIN_All);
}

void sysTimerConfig(void)
{
	// Timer0 for buzzer sound
	Timer0_Stop();												// Stop first, prevent counting
	Timer0_SetMode(TIM_16BitAutoReload);						// Set working mode: 16-bit auto-reload
	Timer0_ClkSelect(TIM_CLOCK_1T);								// Clock source: 1T or 12T
	Timer0_ClkOutput(DISABLE);									// Disable high-speed clock output
	Timer0_Load(65536UL - (MAIN_Fosc / (2*BBEP_FREQUENCY)));	// Initial value: set timer for twice the BEEP frequency
	Timer0_Interrupt(ENABLE);									// Enable interrupt
	Timer0_Priority(NULL);										// No priority configured

	// Timer2 for 16-bit auto-reload, 100Hz, 10 prescaler, interrupt frequency 10Hz
	// Used for simple periodic checks
	Timer2_Stop();												// Stop first, prevent counting
	Timer2_ClkSelect(TIM_CLOCK_12T); 							// Select clock source: TIM_CLOCK_1T, TIM_CLOCK_12T, TIM_CLOCK_Ext
	Timer2_ClkOutput(DISABLE);									// Disable high-speed clock output (ENABLE or DISABLE)
	Timer2_Load(65536UL - (MAIN_Fosc / (NORMAL_FREQUENCE*12)));	// Initial value: set timer
	Timer2_Ps(0);												// Prescaler
	Timer2_Interrupt(ENABLE);									// Enable interrupt
}

void sysI2cConfig(void)
{
	I2cMaster();							// Set as master mode
	I2cClear();								// Clear I2C master status register
	I2cSetSpeed(16);						// Set bus speed: Bus = Fosc/2/(Speed*2+4), valid range 0~63
	I2cAutoSendEnable(DISABLE);				// Enable/disable auto-send (ENABLE or DISABLE)
	I2cEnable(ENABLE);						// Enable/disable I2C functionality (ENABLE or DISABLE)

	I2cMasterInturrupt(DISABLE);			// Disable interrupt; hardware handles operations, status is updated in registers
	I2cCmpPriority(Priority_0);				// Set I2C interrupt priority
	I2CSw(I2C_P33_P32);						// Select which pins to use for I2C
}

#ifdef DEBUG
void sysUartConfig(void)
{
	Uart1Configuration();
	Uart1Interrupt(ENABLE);
	Uart1Priority(Priority_1);
}
#endif

void sysDelayMs(unsigned char ms)
{
	unsigned int i;
	do{
		i = MAIN_Fosc / 10000;
		while(--i);
	}while(--ms);
}
void sysSleepAlarm(u8 enable)
{
	if(enable)
	{
		WKTCL = SLEEP_LOW;
		WKTCH = SLEEP_HIGH;
	}
	else
	{
		WKTCL = 0x00;
		WKTCH = 0x00;
	}
}

void sysSleepMode(void)
{
	// P_SW2 = 0x80;		// Disable extended SFR; when enabling, must turn it on again, otherwise I2C won't recover after sleep
	P0M0 = 0x00;			// Configure P0 pins mode (P0M0 = 0x00 sets push-pull output for all pins)
	P0M1 = 0x00;			// Configure P0 pins mode (P0M1 = 0x00 sets push-pull output for all pins)
	P1M0 = 0X00;
	P1M1 = 0X00;
	P2M0 = 0X00;
	P2M1 = 0X00;
	P3M0 = 0X00;
	P3M1 = 0X00;
	P4M0 = 0X00;
	P4M1 = 0X00;
	P5M0 = 0X00;
	P5M1 = 0X00;
/*
	P0IE=0X00;				//close the digital signal input
	P1IE=0X00;
	P2IE=0X00;
	P3IE=0X00;
	P4IE=0X00;
	P5IE=0X00;
*/
	_nop_();
	_nop_();
	PCON = 0x02;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void sysOpenGpio(void)
{
	P0IE=0XFF;				//open the digital signal input
	P1IE=0XFF;
	P2IE=0XFF;
	P3IE=0XFF;
	P4IE=0XFF;
	P5IE=0XFF;
}


