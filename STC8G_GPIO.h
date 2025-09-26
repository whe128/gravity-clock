#ifndef __STC8G_GPIO_H
#define __STC8G_GPIO_H

#include "STC8G_Register.h"

#define	PIN0						0x01	//IO pin Px.0
#define	PIN1						0x02	//IO pin Px.1
#define	PIN2						0x04	//IO pin Px.2
#define	PIN3						0x08	//IO pin Px.3
#define	PIN4						0x10	//IO pin Px.4
#define	PIN5						0x20	//IO pin Px.5
#define	PIN6						0x40	//IO pin Px.6
#define	PIN7						0x80	//IO pin Px.7
#define	PIN_All						0xFF	//IO pin Px.0~Px.7


//enable pull-up resistor, quasi-bidirectional
#define  GPIO_PULL_UP_P0(Pin)		P0M1 &= ~(Pin), P0M0 &= ~(Pin);
#define  GPIO_PULL_UP_P1(Pin)		P1M1 &= ~(Pin), P1M0 &= ~(Pin);
#define  GPIO_PULL_UP_P2(Pin)		P2M1 &= ~(Pin), P2M0 &= ~(Pin);
#define  GPIO_PULL_UP_P3(Pin)		P3M1 &= ~(Pin), P3M0 &= ~(Pin);
#define  GPIO_PULL_UP_P4(Pin)		P4M1 &= ~(Pin), P4M0 &= ~(Pin);
#define  GPIO_PULL_UP_P5(Pin)		P5M1 &= ~(Pin), P5M0 &= ~(Pin);

//enable push-pull output
#define GPIO_DI_ENABLE_P0(Pin)		P0IE |= (Pin)
#define GPIO_DI_ENABLE_P1(Pin)		P1IE |= (Pin)
#define GPIO_DI_ENABLE_P2(Pin)		P2IE |= (Pin)
#define GPIO_DI_ENABLE_P3(Pin)		P3IE |= (Pin)
#define GPIO_DI_ENABLE_P4(Pin)		P4IE |= (Pin)
#define GPIO_DI_ENABLE_P5(Pin)		P5IE |= (Pin)

//disable push-pull output
#define GPIO_DI_DISENABLE_P0(Pin)	P0IE &= ~(Pin)
#define GPIO_DI_DISENABLE_P1(Pin)	P1IE &= ~(Pin)
#define GPIO_DI_DISENABLE_P2(Pin)	P2IE &= ~(Pin)
#define GPIO_DI_DISENABLE_P3(Pin)	P3IE &= ~(Pin)
#define GPIO_DI_DISENABLE_P4(Pin)	P4IE &= ~(Pin)
#define GPIO_DI_DISENABLE_P5(Pin)	P5IE &= ~(Pin)



#endif
