#ifndef	__CONFIG_H
#define	__CONFIG_H

//========================================================================
//                               Main Clock Definition
//========================================================================

//#define MAIN_Fosc     22118400L   // Define main clock
//#define MAIN_Fosc     12000000L   // Define main clock
//#define MAIN_Fosc     11059200L   // Define main clock
//#define MAIN_Fosc      5529600L   // Define main clock
#define MAIN_Fosc     24000000L   // Define main clock

//========================================================================
//                                  Header Files
//========================================================================
#include "STC8G_Register.h"


//#define DEBUG		    //uartday

#define PIN_BEEP			P37
#define PIN_UP_BUTTON		P10
#define PIN_LEFT_BUTTON		P11
#define PIN_DOWN_BUTTON		P17
#define PIN_RIGHT_BUTTON	P16

#define PIN_BALL_1	 		P34
#define PIN_BALL_2			P35

#define PIN_INT				P36

#define BBEP_FREQUENCY		3000		//HZ
#define	NORMAL_FREQUENCE	100			//Hz-TIM 2 frequency��

#define TIME_CONFIG_UP		1			//1min--HI-
#define TIME_CONFIG_LEFT	15			//15min
#define TIME_CONFIG_DOWN	30			//30min
#define TIME_CONFIG_RIGHT	45			//45min

#define SECOND_PER_MINUTE	60			//s


// Adjust sleep wake-up time ！ UART must be disabled for accurate timing ！ measured on internal 8A16-35350
// Using internal power ！ 86DF ！ 5 min sleep is ~1 min slower


#define SLEEP_LOW				0xF0
#define SLEEP_HIGH				0x85

#define BTN_SET_TRIG_COUNT				2
#define BTN_NO_OPETION_COUNT			4

#define SCAN_DIR_WHILE_COUNT			1
#define SCAN_DIR_TIME2_COUNT			1

#define SCAN_CLOSE_WHILE_COUNT			2
#define SCAN_CLOSE_TIME2_COUNT			3

#define SCAN_BASE_TIME2_COUNT			60

#endif
