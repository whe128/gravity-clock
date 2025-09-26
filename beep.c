#include "beep.h"
#include "config.h"
#include "control.h"
#include "STC8G_Timer.h"

void beepSound(void)					//in TIM_0, for only vibration, use the open/close of TIM_0 to control the beep
{
	PIN_BEEP=~PIN_BEEP;
}

void beepStop(void)
{
	PIN_BEEP=1;
}

void beepControl(void)					//mark some flags, for beepControl strategy
{
	static u8	count;					//minimum cycle count
	static u8	cycle;					//ring for the quick beep
	static u8	total;					//otal cycles time
	static u8	beepFlag;
	static u8	sleepFlag;

	if(!ctrlValue.beepEnable)
	{
		return;
	}

	if(ctrlValue.beepTrigger)			//first time enter (beefFirst), start the ring cycle, initialization
	{
		ctrlValue.beepTrigger=0;
		count=0;
		cycle=0;
		total=0;
		beepFlag=0;
		sleepFlag=0;
	}

	if(count<BEEP_COUNT)
	{
		count++;
	}
	else
	{
		count=0;
		beepFlag=1-beepFlag;
		if(cycle<BEEP_CYCLE)
		{
			cycle++;
		}
		else
		{
			cycle=0;
			sleepFlag=1-sleepFlag;
			if(total<BEEP_TOTAL)
			{
				total++;
			}
			else
			{
				ctrlValue.beepEnable=0;			//complete the beep ring, close
			}
		}
	}
	//反向选择，找出不响铃的
	if(beepFlag==0||sleepFlag==1)
	{
		if(ctrlValue.time0Opened)
		{
			Timer0_Stop();
			ctrlValue.time0Opened=0;
		}
	}
	else
	{
		if(!ctrlValue.time0Opened)
		{
			Timer0_Run();
			ctrlValue.time0Opened=1;
		}
	}
}
