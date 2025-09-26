#include "control.h"
#include "STC8G_Timer.h"
#include "config.h"

#ifdef DEBUG
#include "stdio.h"
#endif


void main(void)
{
	controlSysInit();
	controlOledInit();
	controlInitVaLue();
	controlReadTimeConfig();						// Real-time update of total time to be tracked

	while(1)
	{
		switch(ctrlValue.Status)
		{
			case ST_NONE:			//0
				break;
			case ST_READY:			//1
				controlReady();
				break;
			case ST_ROTATE:			//2
				controlRotate();
				break;
			case ST_TIME:			//3
				controlTime();
				break;
			case ST_BEEP:			//4
				controlBeep();
				break;
			case ST_SLEEP:			//5
				controlSleep();
				break;
			case ST_SET:			//6
				controlSet();
				break;
			case ST_TIME_SET:		//7
				controlTimeSet();
				break;
			default:
				break;
		}



	#ifdef DEBUG
	#if 1

			ctrlValue.WH=0xF8;
			ctrlValue.WL=0xF9;
			if(ctrlValue.debugCount<0xFFFF&&0)
			{
				ctrlValue.debugCount++;
			}
			else
			{
				ctrlValue.debugCount=0;



				printf("ST[%bd]La[%bd] dirSc[%bd]Use[%bd] cloSc[%bd]| btn[up[%bd][%bd][%bd][%bd] ",
					ctrlValue.Status,
					ctrlValue.StatusLast,
					ctrlValue.dirSca,
					ctrlValue.dirUse,
					ctrlValue.closeSca,

					GET_BIT(ctrlValue.button,0),
					GET_BIT(ctrlValue.button,1),
					GET_BIT(ctrlValue.button,2),
					GET_BIT(ctrlValue.button,3)

					);


				printf("timecfg[%bd %bd %bd %bd] timdiv[%hd] oledOpen[%bd] beepT[%bd]En[%bd] ",
					ctrlValue.timeConfig[0],
					ctrlValue.timeConfig[1],
					ctrlValue.timeConfig[2],
					ctrlValue.timeConfig[3],

					ctrlValue.timeCount,

					ctrlValue.oledOpened,
					ctrlValue.beepTrigger,
					ctrlValue.beepEnable

					);

				printf("btnST[%bd]E[%bd] tiT[%bd]whT[%bd] tim0En[%bd]time2En[%bd] HL[%bX][%bX]\n\r",
					ctrlValue.btnSetTrigger,
					ctrlValue.btnSetEnable,
					ctrlValue.scanTimeTigger,
					ctrlValue.scanWhileTigger,
					ctrlValue.time0Opened,
					ctrlValue.time2Opened,
					*(ctrlValue.WH),
					*(ctrlValue.WL)
					);
			}
	#endif

#endif
		controlButtonWhile();

		controlScanWhile();			// Scan direction and close buttons, with debounce
		controlOled();

		controlStepTurn();
	}
}

