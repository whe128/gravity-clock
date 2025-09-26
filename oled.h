#ifndef __OLED_H
#define __OLED_H

#include "sys.h"

#define OLED_CMD	0	        //write command
#define OLED_DATA	1	        //write data
#define OLED_DEV_ADDR	0x78	//OLED device address
#define OLED_CMD_ADDR	0x00
#define	OLED_DATA_ADDR	0x40

void oledDisplayOn(void);
void oledDisplayOff(void);
void oledClear(void);
void oledDisplayTurn(u8 i);


void oledInit(void);
void oledShowTimeConfigH(u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight);		//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight
void oledShowTimeConfigV(u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight);		//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight

void oledShowTimeH(u8 min, u8 sec);
void oledShowTimeV(u8 min, u8 sec);
void oledClearTimeH(void);
void oledClearTimeV(void);


void oledDrawH(u8 i);
void oledDrawV(u8 i);

#endif




