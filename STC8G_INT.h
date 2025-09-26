#ifndef __INT_H
#define __INT_H

#include "STC8G_Register.h"

#define 	INT2_Interrupt(n)		INTCLKO = (INTCLKO & ~0x10) | (n << 4)	/* Enable/disable INT2 interrupt */

#endif
