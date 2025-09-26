#include "STC8G_eeprom.h"

void DisableEEPROM(void)
{
	IAP_CONTR = 0; 			// Disable IAP operations
 	IAP_CMD   = 0; 			// Clear IAP command
 	IAP_TRIG  = 0; 			// Prevent accidental IAP trigger
 	IAP_ADDRH = 0xff; 		// Clear address high byte (set to 0xFF)
 	IAP_ADDRL = 0xff; 		// Clear address low byte (set to 0xFF) to point outside EEPROM area and prevent accidental operations
}

void EEPROMTrig(void)
{
	EA = 0;     			// Disable interrupts to ensure the trigger sequence is not interrupted
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0xA5;        // Send 0x5A then 0xA5 to IAP_TRIG; this sequence is required each time
							// After sending 0xA5 the IAP command is triggered immediately.
							// The CPU waits for the IAP operation to complete before continuing execution.
	_nop_();
	_nop_();
	EA = 1;    // Restore global interrupts
}

//do some modification, start from 0 to save memory, save memory
void EEPROMRead(u8 *DataAddress)
{
	// Modified: start from address 0 to save memory
	IAPEnable();                // Set wait time and enable IAP operations (do once)
	IAPRead();                  // Send byte read command; no need to resend when command doesn't change
	IAP_ADDRH = 0;       		// Send address high byte (only needed when address changes). Start from 0

	IAP_ADDRL = 0;       		// Send address low byte
	EEPROMTrig();               // Trigger EEPROM operation
	*DataAddress = IAP_DATA;    // Store read data

	IAP_ADDRL = 1;       		// Send address low byte
	EEPROMTrig();               // Trigger EEPROM operation
	DataAddress[1] = IAP_DATA;  // Store read data

	IAP_ADDRL = 2;       		// Send address low byte
	EEPROMTrig();               // Trigger EEPROM operation
	DataAddress[2] = IAP_DATA;  // Store read data
	IAP_ADDRL = 3;       		// Send address low byte
	EEPROMTrig();               // Trigger EEPROM operation
	DataAddress[3] = IAP_DATA;  // Store read data

	DisableEEPROM();
}


void EEPROMSectorErase()
{
	IAPEnable();                 // Set wait time and enable IAP operations (do once)
	IAPErase();                  // Macro call: send sector erase command; no need to resend when command doesn't change
	IAP_ADDRH = 0; 	       		 // Send sector address high byte (only needed when address changes)
	IAP_ADDRL = 0; 	       		 // Send sector address low byte
	EEPROMTrig();                // Trigger EEPROM operation
	DisableEEPROM();             // Disable EEPROM (IAP) operations
}

void EEPROMwrite(u8 *DataAddress)
{

	// Erase sector first
	EEPROMSectorErase();

	IAPEnable();                  	// Set wait time and enable IAP operations (do once)
	IAPWrite();                   	// Macro call: send byte write command

	IAP_ADDRH = 0;     			  	// Send address high byte (only needed when address changes)
	IAP_ADDRL = 0;     				// Send address low byte
	IAP_DATA  = *DataAddress;       // Write data to IAP_DATA; only resend when data changes
	EEPROMTrig();                   // Trigger EEPROM operation

	IAP_ADDRL = 1;     				// Send address low byte
	IAP_DATA  = DataAddress[1];     // Write data to IAP_DATA; only resend when data changes
	EEPROMTrig();                   // Trigger EEPROM operation

	IAP_ADDRL = 2;     				// Send address low byte
	IAP_DATA  = DataAddress[2];     // Write data to IAP_DATA; only resend when data changes
	EEPROMTrig();                   // Trigger EEPROM operation

	IAP_ADDRL = 3;     				// Send address low byte
	IAP_DATA  = DataAddress[3];     // Write data to IAP_DATA; only resend when data changes
	EEPROMTrig();                   // Trigger EEPROM operation

	DisableEEPROM();
}


