#include "STC8G_I2C.h"
#include "oled.h"

void Wait()
{
	while (!(I2CMSST & 0x40));					// Wait for interrupt: wait until this bit becomes 1
	I2CMSST &= ~0x40;							// Must manually clear this bit after it becomes 1
}

// Trigger connect
void i2cConnect()
{
	I2CTXD = OLED_DEV_ADDR;						// Write data to data buffer
	I2CMSCR = I2C_ACTION_START_SEND_RECACK;		// Start, send, receive
	Wait();
}

// Trigger command mode
void i2cCmd()
{
	I2CTXD = OLED_CMD_ADDR; 					// Write data to data buffer
	I2CMSCR = I2C_ACTION_SEND_RECACK; 			// Send, receive
	Wait();
}

// Trigger data mode
void i2cData()
{
	I2CTXD = OLED_DATA_ADDR; 					// Write data to data buffer
	I2CMSCR = I2C_ACTION_SEND_RECACK; 			// Send, receive
	Wait();
}
// Can be used to send a command or data
void i2cWriteByte(u8 dat)
{
	I2CTXD = dat;								// Write data to data buffer
	I2CMSCR = I2C_ACTION_SEND_RECACK;			// Send, receive
	Wait();
}

void i2cWriteDataN(u8 *p,u8 number)
{
	i2cConnect();
	i2cData();
	do
	{
		I2CTXD = *p++; 							// Write data to data buffer
		I2CMSCR = I2C_ACTION_SEND_RECACK;		// Send, receive
		Wait();
	}
	while (--number);
	i2cStop();
}

// Remember to stop when finished (call i2cStop)
void i2cStop()
{
	I2CMSCR = I2C_ACTION_STOP;                  // Send STOP command
	Wait();
}

