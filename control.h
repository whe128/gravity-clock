#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"
#include "config.h"

typedef enum
{
	BALL_UP=0,
	BALL_LEFT,
	BALL_DOWN,
	BALL_RIGHT,
	BALL_NONE,
}controlDirection;

typedef enum
{
	ST_NONE=0,		//0
	ST_READY,		//1
	ST_ROTATE,		//2	// Mainly to update time and counting
	ST_TIME,		//3
	ST_BEEP,		//4
	ST_SLEEP,		//5
	ST_SET,			//6
	ST_TIME_SET,	//7
}controlStatus;
/*
typedef enum
{
	BIT_BEEP_TRIGGER=0,			//0
	BIT_BEEP_ENABLE,			//1
	BIT_BTN_SET_TRIGGER,		//2	// Mainly to update time and counting
	BIT_BTN_SET_ENABLE,			//3
	BIT_SCAN_TIME_TRIGGER,		//4
	BIT_SCAN_WHILE_TRIGGER,		//5
	BIT_OLED_OPENED,			//6
	NONE4,						//7
}controlBit;
*/


typedef struct
{
	controlStatus 		Status;
	controlStatus 		StatusLast;		//last status
	controlDirection	dirUse;			//current counting direction

	controlDirection 	dirSca;			//scan for real-time update
	bool				closeSca;		//close scan
	u8					button;			//use low 4 bits to store button status
	//bool				flag;


	bool				beepTrigger;
	bool				beepEnable;
	bool				btnSetTrigger;
	bool				btnSetEnable;
	bool				scanTimeTigger;
	bool				scanWhileTigger;
	bool				oledOpened;
	bool				time0Opened;
	bool				time2Opened;
	u8					timeConfig[4];		//BALL_UP=0,	BALL_LEFT,	BALL_DOWN,	BALL_RIGHT
	u16					timeCount;			// Only decremented once each time, so a single variable is enough

	#ifdef DEBUG
	u16					debugCount;
	u8					intFlag;
	u8					*WH;
	u8					*WL;
	#endif
}controlStruct;

extern controlStruct ctrlValue;

void controlSysInit(void);
void controlReadTimeConfig(void);		// Total time that needs to be tracked in real-time
void controlOledInit(void);
void controlInitVaLue(void);
void controlButtonWhile(void);			// while loop to read direction buttons
void controlbuttonTime2(void);
void controlScanWhile(void);			// Direction and close buttons, with debounce ¡ª mainly for 1s timing in time handling
void controlScanTime2(void);			// Direction and close buttons, with debounce
void controlReady(void);
void controlRotate(void);
void controlTime(void);
void controlBeep(void);
void controlSleep(void);
void controlSet(void);
void controlTimeSet(void);
void controlStepTurn(void);
void controlDrawTimeConfig(void);
void controlOled(void);

#endif



