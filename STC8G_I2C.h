#ifndef __STC8G_I2C_H
#define __STC8G_I2C_H

#include "STC8G_Register.h"
#include "sys.h"

#define I2C_ACTION_NONE						0x00
#define I2C_ACTION_START					0x01
#define I2C_ACTION_SEND						0x02
#define I2C_ACTION_REC_ACK					0x03
#define I2C_ACTION_REC_DATA					0x04
#define I2C_ACTION_SEND_ACK					0x05
#define I2C_ACTION_STOP						0x06
#define I2C_ACTION_START_SEND_RECACK		0x09		// Start command + Send data command + Receive ACK command (for start + device address)
#define I2C_ACTION_SEND_RECACK				0x0A		// Send data command + Receive ACK command (for sending data or commands)

#define I2C_ACTION_RECDATA_SENDACK0			0x0B		// Receive data command + Send ACK(0) command
#define I2C_ACTION_RECDATA_SENDNAK1			0x0C		// Receive data command + Send NAK(1) command



#define	I2C_P14_P15				0
#define	I2C_P24_P25				1
#define	I2C_P33_P32				3

// Interrupt priority
#define		PCMPH		0x20
#define		PCMP		0x20


#define 	I2cInterruptTrigger() 		I2CMSST & 0x40		// I2C action complete interrupt trigger bit
#define		I2cinterruptClear()			I2CMSST &= ~0x40	// Must manually clear this bit after it becomes 1
#define		I2cAck0()					I2CMSST = 0x00
#define		I2cNak1()					I2CMSST = 0x01

#define		I2cBussyStatus()			I2CMSST & 0x80

#define		I2cMaster()					I2CCFG |=  0x40		/* 1: Set as master */
#define 	I2cClear()					I2CMSST = 0x00; 								// Clear I2C master status register
#define		I2cSetSpeed(n)				I2CCFG = (I2CCFG & ~0x3f) | (n & 0x3f)			/* Bus speed = Fosc/2/(Speed*2+4) */
#define		I2cAutoSendEnable(n)		(n==0?(I2CMSAUX &= ~0x01):(I2CMSAUX |= 0x01))				// Enable auto send
#define		I2cEnable(n)				(n==0?(I2CCFG &= ~0x80):(I2CCFG |= 0x80))	// 0: Disable I2C function; 1: Enable I2C function


#define		I2cMasterInturrupt(n)		(n==0?(I2CMSCR &= ~0x80):(I2CMSCR |= 0x80))	// 0: Disable I2C function; 1: Enable I2C function

// Comparator interrupt priority control
#define 	I2cCmpPriority(n)				do{if(n == 0) IP2H &= ~PCMPH, IP2 &= ~PCMP; \
																if(n == 1) IP2H &= ~PCMPH, IP2 |= PCMP; \
																if(n == 2) IP2H |= PCMPH, IP2 &= ~PCMP; \
																if(n == 3) IP2H |= PCMPH, IP2 |= PCMP; \
															}while(0)
// Function pin switching setting
#define  	I2CSw(Pin)					P_SW2 = (P_SW2 & 0xCF) | (Pin << 4)

void i2cStart();
void i2cConnect();
void i2cCmd();
void i2cData();
void i2cWriteByte(u8 dat);
void i2cWriteDataN(u8 *p,u8 number);
void i2cStop();

#endif
