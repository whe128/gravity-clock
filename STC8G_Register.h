#ifndef __STC8G_REGISTER_H
#define __STC8G_REGISTER_H

//GPIOœ‡πÿ
sfr         P0M1        =           0x93;
sfr         P0M0        =           0x94;
sfr         P1M1        =           0x91;
sfr         P1M0        =           0x92;
sfr         P2M1        =           0x95;
sfr         P2M0        =           0x96;
sfr         P3M1        =           0xb1;
sfr         P3M0        =           0xb2;
sfr         P4M1        =           0xb3;
sfr         P4M0        =           0xb4;
sfr         P5M1        =           0xc9;
sfr         P5M0        =           0xca;

sfr         P1          =           0x90;
sbit        P10         =           P1^0;
sbit        P11         =           P1^1;
sbit        P16         =           P1^6;
sbit        P17         =           P1^7;

sfr         P3          =           0xb0;
sbit        P34         =           P3^4;
sbit        P35         =           P3^5;
sbit        P36         =           P3^6;
sbit        P37         =           P3^7;



sfr         TCON        =           0x88;
sbit        TR0         =           TCON^4;
sbit        TR1         =           TCON^6;
sfr         TL0         =           0x8a;
sfr         TH0         =           0x8c;
sfr         T2H         =           0xd6;
sfr         T2L         =           0xd7;
sfr         TH1         =           0x8d;
sfr         TL1         =           0x8b;

sfr         TMOD        =           0x89;
sfr         AUXR        =           0x8e;
sfr         INTCLKO     =           0x8f;
sfr         IE          =           0xa8;
sbit        ET0         =           IE^1;
sbit        ET1         =           IE^3;
sbit        ES          =           IE^4;
sbit        EA          =           IE^7;

sfr         IE2         =           0xaf;

sfr         IPH         =           0xb7;
sfr         IP          =           0xb8;
sbit        PT0         =           IP^1;
sbit        PS          =           IP^4;
sfr         IP2H        =           0xb6;
sfr         IP2         =           0xb5;
sfr         P_SW2       =           0xba;
sfr         SCON        =           0x98;
sbit        TI          =           SCON^1;
sbit        REN         =           SCON^4;
sfr         IAP_ADDRH   =           0xc3;
sfr         IAP_ADDRL   =           0xc4;
sfr         IAP_TRIG    =           0xc6;
sfr         IAP_CONTR   =           0xc7;
sfr         IAP_TPS     =           0xf5;
sfr         IAP_CMD     =           0xc5;
sfr         IAP_DATA    =           0xc2;
sfr         SBUF        =           0x99;
sfr         WKTCL       =           0xaa;
sfr         WKTCH       =           0xab;
sfr         PCON        =           0x87;



#define     P0IE                    (*(unsigned char volatile xdata *)0xfe30)
#define     P1IE                    (*(unsigned char volatile xdata *)0xfe31)
#define     P2IE                    (*(unsigned char volatile xdata *)0xfe32)
#define     P3IE                    (*(unsigned char volatile xdata *)0xfe33)
#define     P4IE                    (*(unsigned char volatile xdata *)0xfe34)
#define     P5IE                    (*(unsigned char volatile xdata *)0xfe35)
#define     TM2PS                   (*(unsigned char volatile xdata *)0xfea2)
#define     I2CCFG                  (*(unsigned char volatile xdata *)0xfe80)
#define     I2CMSST                 (*(unsigned char volatile xdata *)0xfe82)
#define     I2CMSAUX                (*(unsigned char volatile xdata *)0xfe88)
#define     I2CMSCR                 (*(unsigned char volatile xdata *)0xfe81)
#define     I2CTXD                  (*(unsigned char volatile xdata *)0xfe86)
#define     I2CRXD                  (*(unsigned char volatile xdata *)0xfe87)

#define     TMR0_VECTOR             1       //000BH
#define     UART1_VECTOR            4       //0023H
#define     INT2_VECTOR             10      //0053H
#define     TMR2_VECTOR             12      //0063H
#define     CMP_VECTOR              21      //00ABH
#define     I2C_VECTOR              24      //00C3H


#endif
