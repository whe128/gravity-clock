#include "control.h"
#include "STC8G_INT.h"
#include "STC8G_eeprom.h"
#include "STC8G_Timer.h"
#include "oled.h"
#include "beep.h"

#ifdef DEBUG
#include "stdio.h"
#endif

controlStruct ctrlValue;
unsigned char xdata timeOptionUp[9] ={1,2,3,4,5,6,7,8,9};
unsigned char xdata timeOptionLeft[5] ={10,12,15,17,20};
unsigned char xdata timeOptionDown[5] ={30,32,35,37,40};
unsigned char xdata timeOptionRight[5] ={45,50,60,90,120};


void controlSysInit(void)
{
	EAXSFR();		 				// Enable access to extended SFR
	sysGpioConfig();				// Configure GPIO pins
	sysTimerConfig();				// Configure timers (used for sound)
	sysI2cConfig();					// Configure I2C for OLED

#ifdef DEBUG
	sysUartConfig();				// Configure UART for debugging
#endif

	INT2_Interrupt(DISABLE);		// Disable external interrupt INT2 first

	EXIT_ENABLE();                  // Enable global interrupts
}

void controlReadTimeConfig(void)     // Real-time update of total time to be measured
{
	u8 i;

	EEPROMRead(ctrlValue.timeConfig);		//up left down right

#ifdef DEBUG
	//printf("[%bd] [%bd] [%bd] [%bd]\n\r",ctrlValue.timeConfig[0],ctrlValue.timeConfig[1],ctrlValue.timeConfig[2],ctrlValue.timeConfig[3]);
#endif

	for(i=0;i<4;i++)
	{
		// There is unwritten data, rewrite all of it together
		if(ctrlValue.timeConfig[i]==0||ctrlValue.timeConfig[i]==0xFF)
		{
			ctrlValue.timeConfig[BALL_UP]=TIME_CONFIG_UP;
			ctrlValue.timeConfig[BALL_LEFT]=TIME_CONFIG_LEFT;
			ctrlValue.timeConfig[BALL_DOWN]=TIME_CONFIG_DOWN;
			ctrlValue.timeConfig[BALL_RIGHT]=TIME_CONFIG_RIGHT;
			EEPROMwrite(ctrlValue.timeConfig);		//up left down right
			break;
		}
	}
}

void controlOledInit(void)
{
	oledInit();
}

void controlInitVaLue(void)
{
	ctrlValue.Status=ST_NONE;
	ctrlValue.StatusLast=ST_NONE;
	ctrlValue.dirUse=BALL_NONE;
	ctrlValue.dirSca=BALL_NONE;
	ctrlValue.closeSca=0;
	ctrlValue.button=0;
	ctrlValue.timeConfig[0]=0;
	ctrlValue.timeConfig[1]=0;
	ctrlValue.timeConfig[2]=0;
	ctrlValue.timeConfig[3]=0;
	ctrlValue.timeCount=0;

	ctrlValue.beepTrigger=0;
	ctrlValue.beepEnable=0;
	ctrlValue.btnSetTrigger=0;
	ctrlValue.btnSetEnable=0;
	ctrlValue.scanTimeTigger=0;
	ctrlValue.scanWhileTigger=0;
	ctrlValue.oledOpened=0;
	ctrlValue.time0Opened=0;
	ctrlValue.time2Opened=0;

	#ifdef DEBUG
	ctrlValue.intFlag=0;
	#endif
}

u8 controlReadDirection(void)   // Read direction (temporary)
{
	if(PIN_BALL_1)						//is 1
	{
		if(PIN_BALL_2)
		{
			return BALL_UP;
		}
		else
		{
			return BALL_RIGHT;
		}
	}
	else
	{
		if(PIN_BALL_2)
		{
			return BALL_LEFT;
		}
		else
		{
			return BALL_DOWN;
		}
	}
}

u8 controlReadClose(void)							//vcc2, read two omnidirectional sensor
{
	if(PIN_INT)
	{
		return ENABLE;					// High level means tilted or disconnected, disable
	}
	else
	{
		return DISABLE;
	}
}

void controlReadButton(void)							//read four direction buttons
{
	if(PIN_UP_BUTTON)
	{
		SET_BIT(ctrlValue.button, BALL_UP, 0);
	}
	else
	{
		SET_BIT(ctrlValue.button, BALL_UP, 1);
	}

	if(PIN_LEFT_BUTTON)
	{
		SET_BIT(ctrlValue.button, BALL_LEFT, 0);
	}
	else
	{
		SET_BIT(ctrlValue.button, BALL_LEFT, 1);
	}

	if(PIN_DOWN_BUTTON)
	{
		SET_BIT(ctrlValue.button, BALL_DOWN, 0);
	}
	else
	{
		SET_BIT(ctrlValue.button, BALL_DOWN, 1);
	}


	if(PIN_RIGHT_BUTTON)
	{
		SET_BIT(ctrlValue.button, BALL_RIGHT, 0);
	}
	else
	{
		SET_BIT(ctrlValue.button, BALL_RIGHT, 1);
	}
}

u8 controlGetTimeIndexNext(u8 direction, u8 timeOpt[],u8 len)
{
	u8 i;

	for(i=0;i<len;i++)
	{
		if(ctrlValue.timeConfig[direction]==timeOpt[i])				//find the index
		{
			i++;
			break;
		}
	}
	if(i>=len)
	{
		return 0;
	}
	else
	{
		return i;
	}
}

void controlButtonWhile(void)							//while loop to read direction buttons
{
	static u8  countTriggerSet=0, btnTemp=0;
	if(ctrlValue.Status != ST_TIME || ctrlValue.btnSetEnable || ctrlValue.time2Opened)		//stop if trig
	{
		return;
	}

 	controlReadButton();							//read direction

	// Both previous and current readings have value, button is being long-pressed
	if(btnTemp && ctrlValue.button)
	{
		if(countTriggerSet<BTN_SET_TRIG_COUNT)
		{
			countTriggerSet++;
		}
		else
		{
			countTriggerSet=0;
			ctrlValue.btnSetTrigger=1;			// This is the most important
			ctrlValue.btnSetEnable=1;			// This is the most important
		}
	}
	else
	{
		countTriggerSet=0; 						// Reset counting when button is released
	}

	btnTemp=ctrlValue.button;			//Backup
}

void controlbuttonTime2(void)
{
	// temp stores the previous button state, tempTime stores the previous time setting
	// keep is the hold flag
	// NoOperation is the no-operation flag
	static u8 btnTemp,timeTemp[4],count1=0,count2=0;
	u8 i;

	if(!ctrlValue.btnSetEnable)		// Return if not enabled
	{
		return;
	}

	if(ctrlValue.btnSetTrigger)
	{
		ctrlValue.btnSetTrigger=0;
		count1=0;
		count2=0;
		btnTemp=ctrlValue.button;
		for(i=0;i<4;i++)
		{
			timeTemp[i]=ctrlValue.timeConfig[i];
		}
	}

 	controlReadButton();							//read direction
	// read button and update time setting upon button press
	if( !GET_BIT(btnTemp, BALL_UP) && GET_BIT(ctrlValue.button, BALL_UP))	//Can only trigger once; must release before triggering again
	{
		i=controlGetTimeIndexNext(BALL_UP,timeOptionUp,sizeof(timeOptionUp));
		ctrlValue.timeConfig[BALL_UP]=timeOptionUp[i];
	}

	if( !GET_BIT(btnTemp, BALL_LEFT) && GET_BIT(ctrlValue.button, BALL_LEFT))	//Can only trigger once; must release before triggering again
	{
		i=controlGetTimeIndexNext(BALL_LEFT,timeOptionLeft,sizeof(timeOptionLeft));
		ctrlValue.timeConfig[BALL_LEFT]=timeOptionLeft[i];
	}

	if( !GET_BIT(btnTemp, BALL_DOWN) && GET_BIT(ctrlValue.button, BALL_DOWN))	//Can only trigger once; must release before triggering again
	{
		i=controlGetTimeIndexNext(BALL_DOWN,timeOptionDown,sizeof(timeOptionDown));
		ctrlValue.timeConfig[BALL_DOWN]=timeOptionDown[i];
	}

	if( !GET_BIT(btnTemp, BALL_RIGHT) && GET_BIT(ctrlValue.button, BALL_RIGHT))	//Can only trigger once; must release before triggering again
	{
		i=controlGetTimeIndexNext(BALL_RIGHT,timeOptionRight,sizeof(timeOptionRight));
		ctrlValue.timeConfig[BALL_RIGHT]=timeOptionRight[i];
	}

	//Exit after long period of inactivity ¡ª assume no operation performed ¡ª shift left 2 bits, discard top 2 bits
	if(btnTemp||ctrlValue.button)
	{
		// Button pressed, reset exit counter
		count1=0;
		count2=0;
	}
	else
	{
		if(count1<99)
		{
			count1++;
		}
		else
		{
			count1=0;
			if(count2<BTN_NO_OPETION_COUNT)			//Whichever reaches first triggers
			{
				count2++;
			}
			else
			{
				count2=0;
				ctrlValue.btnSetEnable=0;				//finish setting
			}
		}
	}

	if(!ctrlValue.btnSetEnable)		//Return immediately if not previously enabled
	{
		//Loop to find a different value, then write to EEPROM
		for(i=0;i<4;i++)
		{
			if(ctrlValue.timeConfig[i]!=timeTemp[i])
			{
				EEPROMwrite(ctrlValue.timeConfig);		//up left down right
				break;
			}
		}

		// If previous values are the same, EEPROM is not written.
		// If different, write and update temp to prevent multiple writes, ensuring temp and config stay synchronized after writing
		for(i=0;i<4;i++)
		{
			timeTemp[i]=ctrlValue.timeConfig[i];
		}
	}

	// Read each cycle and record the previous button state
	btnTemp=ctrlValue.button;
}



void controlScanWhile(void)			// Scan direction and close buttons, with debounce ¡ª mainly for 1s timing in time handling
{
	static u8 dirTemp=0,dirTempLast=0, closeTemp=0,closeTempLast=0, countdir=0,countClose=0;
	if(ctrlValue.time2Opened)
	{
		return;
	}
	if(ctrlValue.scanWhileTigger)
	{
		ctrlValue.scanWhileTigger=0;
		dirTemp=BALL_NONE;
		dirTempLast=BALL_NONE;
		closeTemp=0;
		closeTempLast=0;
		countdir=0;
		countClose=0;
	}

	dirTemp=controlReadDirection();
	closeTemp=controlReadClose();

	// If directions differ between two readings, reset counter; direction state remains unchanged
	if(dirTemp!=dirTempLast)
	{
		countdir=0;
	}
	else
	{
		if(countdir<SCAN_DIR_WHILE_COUNT)
		{
			countdir++;
		}
		else
		{
			countdir=0;
			ctrlValue.dirSca=dirTemp;
		}
	}

	// Both readings are 1
	if(closeTemp && closeTempLast)
	{
		if(countClose<SCAN_CLOSE_WHILE_COUNT)
		{
			countClose++;
		}
		else
		{
			countClose=0;
			ctrlValue.closeSca=1;				 // Easier to switch open, harder to switch close
		}
	}
	else
	{
		ctrlValue.closeSca=0;
		countClose=0;
	}

	//backup
	dirTempLast=dirTemp;
	closeTempLast=closeTemp;
}
void controlScanTime2(void)			//Scan direction and close buttons, with debounce
{
	static u8 dirTemp=0,dirTempLast=0, closeTemp=0,closeTempLast=0;
	static u8 countdir=0,countdirBase=0,countClose=0,countCloseBase=0;


	if(ctrlValue.scanTimeTigger)
	{
		ctrlValue.scanTimeTigger=0;
		dirTemp=BALL_NONE;
		dirTempLast=BALL_NONE;
		closeTemp=0;
		closeTempLast=0;
		countdir=0;
		countClose=0;
	}

	dirTemp=controlReadDirection();
	closeTemp=controlReadClose();

	// Directions differ between two readings, reset counter
	if(dirTemp!=dirTempLast)
	{
		countdirBase=0;
		countdir=0;
	}
	else
	{
		if(countdirBase<SCAN_BASE_TIME2_COUNT)
		{
			countdirBase++;
		}
		else
		{
			countdirBase=0;
			if(countdir<SCAN_DIR_WHILE_COUNT)
			{
				countdir++;
			}
			else
			{
				countdir=0;
				ctrlValue.dirSca=dirTemp;
			}
		}
	}

	//Both readings are 1
	if(closeTemp && closeTempLast)
	{
		if(countCloseBase<SCAN_BASE_TIME2_COUNT)
		{
			countdirBase++;
		}
		else
		{
			countdirBase=0;
			if(countClose<SCAN_CLOSE_WHILE_COUNT)
			{
				countClose++;
			}
			else
			{
				countClose=0;
				ctrlValue.closeSca=1;
			}
		}
	}
	else
	{
		countClose=0;
		countCloseBase=0;
		ctrlValue.closeSca=0;
	}

	//backup
	dirTempLast=dirTemp;
	closeTempLast=closeTemp;
}

void controlReady(void)
{
	if(!ctrlValue.time2Opened)
	{
		ctrlValue.scanTimeTigger=1;
		Timer2_Run();
		ctrlValue.time2Opened=1;
	}
}

void controlRotate(void)
{
	ctrlValue.dirUse=ctrlValue.dirSca;														// First entry
	ctrlValue.timeCount=ctrlValue.timeConfig[ctrlValue.dirUse]*SECOND_PER_MINUTE;			// Update counter


	// Beep is not activated now; setButton will use them
	if((ctrlValue.StatusLast==ST_TIME_SET && !ctrlValue.btnSetEnable) || ctrlValue.StatusLast!=ST_TIME_SET)
	{
		if(ctrlValue.time0Opened)
		{
			Timer0_Stop();
			ctrlValue.time0Opened=0;
		}
		if(ctrlValue.time2Opened)
		{
			ctrlValue.scanWhileTigger=1;
			Timer2_Stop();
			ctrlValue.time2Opened=0;
		}
		beepStop();
		sysSleepAlarm(TRUE);
	}
}
void controlTime(void)
{
	if(ctrlValue.timeCount>0)									// Enter power-down mode when not zero
	{
		ctrlValue.timeCount--;
		sysSleepMode();
	}
	else
	{
		sysSleepAlarm(FALSE);
	}

}
void controlBeep(void)
{
	if(ctrlValue.StatusLast!=ST_BEEP)						// Previous state was not
	{
		ctrlValue.beepTrigger=1;
		ctrlValue.beepEnable=1;

		if(!ctrlValue.time2Opened)
		{
			ctrlValue.scanTimeTigger=1;
			Timer2_Run();
			ctrlValue.time2Opened=1;
		}
	}
}
void controlSleep(void)
{
	if(ctrlValue.time0Opened)
	{
		Timer0_Stop();
		ctrlValue.time0Opened=0;
	}
	if(ctrlValue.time2Opened)
	{
		ctrlValue.scanWhileTigger=1;
		Timer2_Stop();
		ctrlValue.time2Opened=0;
	}
	beepStop();

	if(ctrlValue.oledOpened)
	{
		oledClear();
		oledDisplayOff();
		ctrlValue.oledOpened=0;
	}


	ctrlValue.dirSca=BALL_NONE;
	ctrlValue.dirUse=BALL_NONE;
	ctrlValue.closeSca=0;
	sysSleepAlarm(FALSE);			// Do not wake up automatically

	// Cannot be placed inside sleepMode, because it will be called by time--
	INT2_Interrupt(ENABLE);
	sysSleepMode();
	INT2_Interrupt(DISABLE);
}
void controlSet(void)
{
	sysOpenGpio();
}

void controlTimeSet(void)
{
	if(ctrlValue.StatusLast != ST_TIME_SET)
	{
		if(!ctrlValue.time2Opened)
		{
			sysSleepAlarm(FALSE);			// Because it is entered from time
			ctrlValue.scanTimeTigger=1;
			Timer2_Run();
			ctrlValue.time2Opened=1;
		}
	}
}
void controlStepTurn(void)
{
	// Assign first, then update; can only be used when running at the beginning of the function,
	// later it will be overwritten
	ctrlValue.StatusLast=ctrlValue.Status;


	// Cannot use Last as a condition here, because it has been replaced
	// and the new value hasn't been assigned yet.
	// Ready, time, beep, Timeset will run multiple loops
	switch(ctrlValue.Status)
	{
		case ST_NONE:
			ctrlValue.Status=ST_READY;
			break;
		case ST_READY:
			if(ctrlValue.dirSca!=BALL_NONE)
			{
				ctrlValue.Status=ST_ROTATE;
			}
			break;
		case ST_ROTATE:
			if(ctrlValue.btnSetEnable)
			{
				ctrlValue.Status=ST_TIME_SET;
			}
			else
			{
				ctrlValue.Status=ST_TIME;
			}
			break;
		case ST_TIME:
			if(ctrlValue.btnSetEnable)
			{
				ctrlValue.Status=ST_TIME_SET;
			}
			else if(ctrlValue.dirUse != ctrlValue.dirSca && !controlReadClose())		//not switch direction when read 0
			{
				ctrlValue.Status=ST_ROTATE;
			}
			else if(!ctrlValue.timeCount)
			{
				ctrlValue.Status=ST_BEEP;
			}
			break;
		case ST_BEEP:
			if(ctrlValue.dirUse != ctrlValue.dirSca && !controlReadClose())		// Reset when direction changes; it is still open at this time

			{
				ctrlValue.Status=ST_ROTATE;
			}
			else if(!ctrlValue.beepEnable)				//beep ring complete
			{
				ctrlValue.Status=ST_SLEEP;
			}
			break;
		case ST_SLEEP:
			ctrlValue.Status=ST_SET;
			break;
		case ST_SET:
			ctrlValue.Status=ST_READY;
			break;
		case ST_TIME_SET:
			if(!ctrlValue.btnSetEnable || ctrlValue.dirUse != ctrlValue.dirSca)
			{
				if(!controlReadClose())
				{
					ctrlValue.Status=ST_ROTATE;
				}
			}
			break;
	}

	if(ctrlValue.closeSca)
	{
		ctrlValue.Status=ST_SLEEP;
	}
}

void controlDrawTimeConfig(void)
{
	switch(ctrlValue.dirUse)
	{
		case BALL_UP:
			oledShowTimeConfigH(ctrlValue.timeConfig[BALL_UP],ctrlValue.timeConfig[BALL_LEFT], ctrlValue.timeConfig[BALL_DOWN], ctrlValue.timeConfig[BALL_RIGHT]);			//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight
			break;

		case BALL_LEFT:
			oledShowTimeConfigV(ctrlValue.timeConfig[BALL_LEFT],ctrlValue.timeConfig[BALL_DOWN], ctrlValue.timeConfig[BALL_RIGHT], ctrlValue.timeConfig[BALL_UP]);			//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight
			break;

		case BALL_DOWN:
			oledShowTimeConfigH(ctrlValue.timeConfig[BALL_DOWN],ctrlValue.timeConfig[BALL_RIGHT], ctrlValue.timeConfig[BALL_UP], ctrlValue.timeConfig[BALL_LEFT]);			//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight
			break;

		case BALL_RIGHT:
			oledShowTimeConfigV(ctrlValue.timeConfig[BALL_RIGHT],ctrlValue.timeConfig[BALL_UP], ctrlValue.timeConfig[BALL_LEFT], ctrlValue.timeConfig[BALL_DOWN]);			//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight
			break;

		default:
			break;
	}
}

void controlOled(void)
{
	static u8 tempTimeDrawed[4];
	u8 i;


	switch(ctrlValue.Status)
	{
		case ST_NONE:
		case ST_READY:
		case ST_SET:
		case ST_BEEP:
		case ST_SLEEP:
			return;
			break;
		default:
			break;
	}


	if(!ctrlValue.oledOpened)
	{
		oledDisplayOn();
		ctrlValue.oledOpened=1;
	}

	if(ctrlValue.Status==ST_ROTATE)
	{
		// Every time entering rotate, the display must be cleared
		oledClear();

		// Draw background
		switch(ctrlValue.dirUse)
		{
			case BALL_UP:
				oledDisplayTurn(FALSE);
				oledDrawH(BALL_UP);
				break;
			case BALL_LEFT:
				oledDisplayTurn(FALSE);
				oledDrawV(BALL_LEFT);
				break;
			case BALL_DOWN:
				oledDisplayTurn(TRUE);
				oledDrawH(BALL_DOWN);
				break;
			case BALL_RIGHT:
				oledDisplayTurn(TRUE);
				oledDrawV(BALL_RIGHT);
				break;
			default:
				break;
		}

		//draw time config
		controlDrawTimeConfig();

		// Backup the timeConfig that was drawn
		for(i=0;i<4;i++)
		{
			tempTimeDrawed[i]=ctrlValue.timeConfig[i];
		}
	}


	if(ctrlValue.Status==ST_TIME||(ctrlValue.Status==ST_ROTATE && !ctrlValue.btnSetEnable))
	{
		if(ctrlValue.dirUse==BALL_UP||ctrlValue.dirUse==BALL_DOWN)
		{
			oledShowTimeH(ctrlValue.timeCount/60, ctrlValue.timeCount%60);
		}
		else
		{
			oledShowTimeV(ctrlValue.timeCount/60, ctrlValue.timeCount%60);
		}
	}

	else if(ctrlValue.Status==ST_TIME_SET)
	{
		if(ctrlValue.StatusLast==ST_TIME)
		{
			if(ctrlValue.dirUse==BALL_UP||ctrlValue.dirUse==BALL_DOWN)
			{
				oledClearTimeH();
			}
			else
			{
				oledClearTimeV();
			}
		}
		else if(ctrlValue.btnSetEnable)
		{
			for(i=0;i<4;i++)
			{
				if(tempTimeDrawed[i]!=ctrlValue.timeConfig[i])
				{
					controlDrawTimeConfig();

					// Must be placed here, otherwise it would be updated as well and not displayed
					for(i=0;i<4;i++)
					{
						tempTimeDrawed[i]=ctrlValue.timeConfig[i];
					}
					break;
				}
			}

					// Backup the drawn timeConfig, must be placed outside the loop
		}
	}
}


