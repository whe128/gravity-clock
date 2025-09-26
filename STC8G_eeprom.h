#ifndef	__STC8G_H_EEPROM_H
#define	__STC8G_H_EEPROM_H

#include "STC8G_Register.h"
#include "config.h"
#include "sys.h"


#define		IAPEnable()		IAP_CONTR = (1<<7); IAP_TPS = MAIN_Fosc / 1000000
#define		IAPRead()		IAP_CMD = 1		//IAP read command
#define		IAPWrite()		IAP_CMD = 2		//IAP write command
#define		IAPErase()		IAP_CMD = 3		//IAP erase command


void EEPROMRead(u8 *DataAddress);
void EEPROMwrite(u8 *DataAddress);

#endif
