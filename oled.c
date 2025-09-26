
#include "STC8G_I2C.h"
#include "oled.h"
#include "oledfont.h"
#include "sys.h"

// Send fewer commands; finish each transmission to avoid issues
void oledSendCMD(u8 cmd)  /*  DeviceAddress,WordAddress,First Data Address,Byte lenth   */
{
	i2cConnect();
	i2cCmd();
	i2cWriteByte(cmd);
	i2cStop();
}

//open OLED display
void oledDisplayOn(void)
{
	oledSendCMD(0X8D);
	oledSendCMD(0X14);
	oledSendCMD(0XAF);
}


//close OLED display
void oledDisplayOff(void)
{
	oledSendCMD(0X8D);
	oledSendCMD(0X10);
	oledSendCMD(0XAE);
}

void oledClear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		oledSendCMD(0xb0+i);			//set page address (0~7)
		oledSendCMD(0x00);				//set lower column address
		oledSendCMD(0x10);				//set higher column address


		i2cConnect();
		i2cData();
		for(n=0;n<128;n++)
		{
			i2cWriteByte(0);
		}
		i2cStop();
	} //update display
}

//rotate display 180 degrees
void oledDisplayTurn(u8 i)
{
	if(i==0)
	{
		oledSendCMD(0xC8);	//normal display
		oledSendCMD(0xA1);
	}
	else
	{
		oledSendCMD(0xC0);	//Invert display
		oledSendCMD(0xA0);
	}
}


// Set coordinates — Y coupling, initialization done externally, because characters follow
void oledSetPos(u8 x, u8 y)
{
	oledSendCMD(0xb0+y);
	oledSendCMD(((x&0xf0)>>4)|0x10);
	oledSendCMD(x&0x0f);
}

/*
void oledShowChar8x6H(u8 x,u8 y,u8 font[])				//h is page number
{
	oledSetPos(x,y);
	i2cWriteDataN(font,6);
}
void oledShowChar16x8H(u8 x,u8 y,u8 font[])				//h is page number
{
	oledSetPos(x,y);
	i2cWriteDataN(font,8);

	oledSetPos(x,y+1);
	i2cWriteDataN(font+8,8);
}
void oledShowChar8x8V(u8 x,u8 y,u8 font[])				//h is page number
{
	oledSetPos(x,y);		//设一次就好了
	i2cWriteDataN(font,8);
}

void oledShowChar16x8V(u8 x,u8 y,u8 font[])				//h is page number
{
	oledSetPos(x,y);		//设一次就好了
	i2cWriteDataN(font,16);
}
*/


// Ignore vertical alignment
void oledShowBMP(u8 x,u8 y,u8 w, u8 h,u8 bmp[])			//h is page number
{
	//i for the iteration of height, j for width
	u8 i,j;

	for(i=0;i<h;i++)
	{
		oledSetPos(x,y+i);		//set position each time
		if(bmp)
		{
			i2cWriteDataN(bmp+(i*w),w);	//draw each
		}
		else
		{
			i2cConnect();
			i2cData();
			for(j=0;j<w;j++)
			{
				i2cWriteByte(0);		// Send as cleanup when character pointer is NULL
			}
			i2cStop();
		}
	}
}


// Initialize
void oledInit(void)
{
  	//sysDelayMs(200);
  	sysDelayMs(50);


	oledSendCMD(0xAE);	// Turn off display (enter sleep mode) to prepare configuration

	oledSendCMD(0x00);	// Set column start address (low)
	oledSendCMD(0x10);	// Set column start address (high)

	oledSendCMD(0x40);	// Set start line address: 0x40 = line 0, 0x7F = line 63

	oledSendCMD(0x81);	// Set contrast
	oledSendCMD(0x1F);	// Contrast value: 0x00-0xFF (256 levels)

	oledSendCMD(0xA1);	// 0xA1 normal display, 0xA0 left-right flip
	oledSendCMD(0xC8);	// 0xC8 normal display, 0xC0 top-bottom flip

	oledSendCMD(0xA6);	// 0xA6 normal display, 0xA7 inverse display

	oledSendCMD(0xA8);	// Set multiplex ratio
	oledSendCMD(0x3f);	// 0x00-0x3F corresponds to 0-64

	oledSendCMD(0xD3);	// Set display offset (0x00-0x3F)
	oledSendCMD(0x00);	// 0x00-0x3F

	oledSendCMD(0xd5);	// Set divide ratio and oscillator frequency
	oledSendCMD(0x80);	// (Low 4 bits: divide ratio, High 3 bits: oscillator frequency) -- set divide ratio, Set Clock as 100 Frames/Sec

	oledSendCMD(0xD9);	// Set pre-charge period
	oledSendCMD(0xF1);	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

	oledSendCMD(0xDA);	// Set COM pin hardware configuration
	oledSendCMD(0x12);	// 0x12 use alternate pin configuration and disable left-right remap

	oledSendCMD(0xDB);	// Set VCOMH voltage
	oledSendCMD(0x40);	// Set VCOM Deselect Level

	oledSendCMD(0x20);	// Set memory addressing mode
	oledSendCMD(0x02);	// 0x00 horizontal mode, 0x01 vertical mode, 0x02 page addressing mode

	oledSendCMD(0x8D);	// Set charge pump
	oledSendCMD(0x14);	// 0x14 enable charge pump, 0x10 disable charge pump

	oledSendCMD(0xA4);	// 0xA4 display RAM content, 0xA5 ignore RAM and fill entire screen

	oledSendCMD(0xA6);	// 0xA6 normal display, 0xA7 inverse display

	oledClear();

	//oledSendCMD(0xAF);	// Turn on screen

}

// Here, directions refer to the current pointing direction
void oledShowTimeConfigH(u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight)				//u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight
{

	// Assume up to 3 digits
	u8 hun,ten,unit;

	// Clear regions before drawing digits
	oledShowBMP(54,0,18,1,NULL);
	oledShowBMP(0,1,18,1,NULL);
	oledShowBMP(54,2,18,1,NULL);
	oledShowBMP(109,1,18,1,NULL);


	//up
	hun=timeUp/100;
	ten=(timeUp%100)/10;
	unit=timeUp%10;
	if(hun)
	{
		oledShowBMP(54,0,6,1,number_H_8x6[hun]);
		oledShowBMP(60,0,6,1,number_H_8x6[ten]);
		oledShowBMP(66,0,6,1,number_H_8x6[unit]);
	}
	else if(ten)
	{
		oledShowBMP(57,0,6,1,number_H_8x6[ten]);
		oledShowBMP(63,0,6,1,number_H_8x6[unit]);
	}
	else
	{
		oledShowBMP(63,0,6,1,number_H_8x6[unit]);
	}


	//left
	hun=timeLeft/100;
	ten=(timeLeft%100)/10;
	unit=timeLeft%10;
	if(hun)
	{
		oledShowBMP(0,1,6,1,number_H_8x6[hun]);
		oledShowBMP(6,1,6,1,number_H_8x6[ten]);
		oledShowBMP(12,1,6,1,number_H_8x6[unit]);
	}
	else if(ten)
	{
		oledShowBMP(0,1,6,1,number_H_8x6[ten]);
		oledShowBMP(6,1,6,1,number_H_8x6[unit]);
	}
	else
	{
		oledShowBMP(0,1,6,1,number_H_8x6[unit]);
	}


	//down
	hun=timeDown/100;
	ten=(timeDown%100)/10;
	unit=timeDown%10;
	if(hun)
	{
		oledShowBMP(54,2,6,1,number_H_8x6[hun]);
		oledShowBMP(60,2,6,1,number_H_8x6[ten]);
		oledShowBMP(66,2,6,1,number_H_8x6[unit]);
	}
	else if(ten)
	{
		oledShowBMP(57,2,6,1,number_H_8x6[ten]);
		oledShowBMP(63,2,6,1,number_H_8x6[unit]);
	}
	else
	{
		oledShowBMP(60,2,6,1,number_H_8x6[unit]);
	}

	//right
	hun=timeRight/100;
	ten=(timeRight%100)/10;
	unit=timeRight%10;
	if(hun)
	{
		oledShowBMP(109,1,6,1,number_H_8x6[hun]);
		oledShowBMP(115,1,6,1,number_H_8x6[ten]);
		oledShowBMP(121,1,6,1,number_H_8x6[unit]);
	}
	else if(ten)
	{
		oledShowBMP(115,1,6,1,number_H_8x6[ten]);
		oledShowBMP(121,1,6,1,number_H_8x6[unit]);
	}
	else
	{
		oledShowBMP(121,1,6,1,number_H_8x6[unit]);
	}

}


void oledShowTimeConfigV(u8 timeUp, u8 timeLeft, u8 timeDown, u8 timeRight)
{
	// Assume up to 3 digits
	u8 hun,ten,unit;

	// Clear regions before drawing digits
	oledShowBMP(0, 3,8,3,NULL);
	oledShowBMP(16,5,8,3,NULL);
	oledShowBMP(32,3,8,3,NULL);
	oledShowBMP(16,0,8,3,NULL);

	// up
	hun=timeUp/100;
	ten=(timeUp%100)/10;
	unit=timeUp%10;
	if(hun)
	{
		oledShowBMP(0,5,8,1,number_V_8x8[hun]);
		oledShowBMP(0,4,8,1,number_V_8x8[ten]);
		oledShowBMP(0,3,8,1,number_V_8x8[unit]);
	}
	else if(ten)
	{
		oledShowBMP(0,4,8,1,number_V_8x8[ten]);
		oledShowBMP(0,3,8,1,number_V_8x8[unit]);
	}
	else
	{
		oledShowBMP(0,4,8,1,number_V_8x8[unit]);
	}

	// left
	hun=timeLeft/100;
	ten=(timeLeft%100)/10;
	unit=timeLeft%10;
	if(hun)
	{
		oledShowBMP(16,7,8,1,number_V_8x8[hun]);
		oledShowBMP(16,6,8,1,number_V_8x8[ten]);
		oledShowBMP(16,5,8,1,number_V_8x8[unit]);
	}
	else if(ten)
	{
		oledShowBMP(16,7,8,1,number_V_8x8[ten]);
		oledShowBMP(16,6,8,1,number_V_8x8[unit]);
	}
	else
	{
		oledShowBMP(16,7,8,1,number_V_8x8[unit]);
	}

	//down
	hun=timeDown/100;
	ten=(timeDown%100)/10;
	unit=timeDown%10;
	if(hun)
	{
		oledShowBMP(32,5,8,1,number_V_8x8[hun]);
		oledShowBMP(32,4,8,1,number_V_8x8[ten]);
		oledShowBMP(32,3,8,1,number_V_8x8[unit]);
	}
	else if(ten)
	{
		oledShowBMP(32,4,8,1,number_V_8x8[ten]);
		oledShowBMP(32,3,8,1,number_V_8x8[unit]);
	}
	else
	{
		oledShowBMP(32,4,8,1,number_V_8x8[unit]);
	}

	// right
	hun=timeRight/100;
	ten=(timeRight%100)/10;
	unit=timeRight%10;
	if(hun)
	{
		oledShowBMP(16,2,8,1,number_V_8x8[hun]);
		oledShowBMP(16,1,8,1,number_V_8x8[ten]);
		oledShowBMP(16,0,8,1,number_V_8x8[unit]);
	}
	else if(ten)
	{
		oledShowBMP(16,1,8,1,number_V_8x8[ten]);
		oledShowBMP(16,0,8,1,number_V_8x8[unit]);
	}
	else
	{
		oledShowBMP(16,0,8,1,number_V_8x8[unit]);
	}
}


void oledShowTimeH(u8 min, u8 sec)
{
	// Assume up to 3 digits -- colon at 59-67

	// min
	if(min>99)
	{
		oledShowBMP(35,4,8,2,char_H_16x8[min/100]);
		oledShowBMP(43,4,8,2,char_H_16x8[(min%100)/10]);
		oledShowBMP(51,4,8,2,char_H_16x8[min%10]);
	}
	else if(min>9)
	{
		if(min==99)		// When min is 99, clear hundreds place
		{
			oledShowBMP(35,4,8,2,NULL);
		}
		oledShowBMP(43,4,8,2,char_H_16x8[min/10]);
		oledShowBMP(51,4,8,2,char_H_16x8[min%10]);
	}
	else
	{
		if(min==9)		// When min is 9, clear tens place
		{
			oledShowBMP(43,4,8,2,NULL); 			// When showing units digit, hundreds place was already cleared when tens were cleared
		}
		oledShowBMP(51,4,8,2,char_H_16x8[min]);
	}

	// Always display seconds
	oledShowBMP(67,4,8,2,char_H_16x8[sec/10]);
	oledShowBMP(75,4,8,2,char_H_16x8[sec%10]);

}

void oledShowTimeV(u8 min, u8 sec)
{
	// Assume up to 3 digits -- colon at 59-67

	// min
	if(min>99)
	{
		oledShowBMP(59,6,16,1,char_V_16x8[min/100]);
		oledShowBMP(59,5,16,1,char_V_16x8[(min%100)/10]);
		oledShowBMP(59,4,16,1,char_V_16x8[min%10]);
	}
	else if(min>9)
	{
		if(min==99)
		{
			oledShowBMP(59,6,16,1,NULL);
		}
		oledShowBMP(59,5,16,1,char_V_16x8[min/10]);
		oledShowBMP(59,4,16,1,char_V_16x8[min%10]);
	}
	else
	{
		if(min==9)
		{
			oledShowBMP(59,5,16,1,NULL);
		}
		oledShowBMP(59,4,16,1,char_V_16x8[min]);
	}

	// Always display seconds
	oledShowBMP(59,2,16,1,char_V_16x8[sec/10]);
	oledShowBMP(59,1,16,1,char_V_16x8[sec%10]);

}

void oledClearTimeH(void)
{
	oledShowBMP(35,4, 24, 2, NULL); 			// h is page count
	oledShowBMP(67,4, 16, 2, NULL); 			// h is page count
}
void oledClearTimeV(void)
{
	oledShowBMP(59,1, 16, 2, NULL); 			// h is page count
	oledShowBMP(59,4, 16, 3, NULL); 			// h is page count
}

void oledDrawH(u8 i)
{
	oledShowBMP(39, 7,6,1, char_H_8x6[0]);	//D
	oledShowBMP(45, 7,6,1, char_H_8x6[1]);	//e
	oledShowBMP(51, 7,6,1, char_H_8x6[2]);	//s
	oledShowBMP(57, 7,6,1, char_H_8x6[3]);	//i
	oledShowBMP(63, 7,6,1, char_H_8x6[4]);	//g
	oledShowBMP(69, 7,6,1, char_H_8x6[5]);	//n
	oledShowBMP(81, 7,6,1, char_H_8x6[6]);	//f
	oledShowBMP(87, 7,6,1, char_H_8x6[7]);	//o
	oledShowBMP(93, 7,6,1, char_H_8x6[8]);	//r
	oledShowBMP(110, 6,8,2, char_H_16x8[11]);	// Small character
	oledShowBMP(119, 6,8,2, char_H_16x8[12]);	// Beauty character
	oledShowBMP(0,5,24, 3,bmp_direction[i]);	// Pattern
	oledShowBMP(59, 4,8,2,char_H_16x8[10]);		// Colon
}



void oledDrawV(u8 i)
{
	//design for
	oledShowBMP(119,0,8, 8,bmp_desinFor);
	oledShowBMP(103,1,16,1,char_V_16x8[11]);	// Small character
	oledShowBMP(103,0,16,1,char_V_16x8[12]);	// Beauty character
	oledShowBMP(92,5,24,3,bmp_direction[i]) ;	// Pattern
	oledShowBMP(58,3,16,1,char_V_16x8[10]);		// Colon
}


