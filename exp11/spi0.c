#include <LPC214x.h>
#include "utils.h"
#include "spi0.h"
#include "uart.h"
// refer s0spsr & its lastbit
#define S0PINSEL0_PINS 0x0000ff00
#define SPIFBIT 0x80
#define MASTER_CSPIN 7 // must be on port 1 NOTE: ADJUST PINSEL TODO
char ISMASTER;
int __intupdate = 0,BUFFERTAIL=0,BUFFERHEAD=0;
char RX, TX,SPI_BUFFER[100];

void spi0_masterinit()
{
    ISMASTER = 1;
    PINSEL0 &= ~S0PINSEL0_PINS;  // cleared all bits for  uart0
    PINSEL0 |= 0x00001500;       // Master 						0.7 as gpio
    IO0DIR |= 1 << MASTER_CSPIN; // set the gpio pin to output

    // SPI0 rate may be calculated as: PCLK / SPCCR0 value // modes also here
    S0SPCCR = 0xA;   // Set SPI clock counter register - clock rate			         // sdo   0.6
    S0SPCR = 0X0020; // 20 for master, control register ; master/slave select;  	// sd1 	  0.5

    //  VICIntSelect &= ~(1 << 10);
    //  VICVectCntl8 = 10 | (1 << 5);                  // set to spi0 & int10
    //  VICVectAddr8 = (unsigned long int)spi0routine; // sck	 0.4
    //  Read SPI status
    //  SPI data (received)
    //  S0SPCR |=0x80; // int enable on peripheral
}

void spi0_slavecntrl(char enable)
{
    // assumes pin 7 tobe on the gpio mode to control slave
    if (enable == 1)
        IO0CLR = (1 << MASTER_CSPIN);
    else
        IO0SET = (1 << MASTER_CSPIN);
}

char spi0_masterwrite(char data)
{
    spi0_slavecntrl(1); // pull cs low
    S0SPDR = data;
    while ((S0SPSR & SPIFBIT) == 0)
        ;               // waitng for data transfer complete
    spi0_slavecntrl(0); // pull cs high
    return S0SPDR;      // yup,write function can also be used to read the recived data
}

char spi0_masterread()
{                       // exclusive read function
    spi0_slavecntrl(1); // pull cs low
    S0SPDR = 0xAB;      // flushing something out
    while ((S0SPSR & SPIFBIT) == 0)
        ;               // waitng for data transfer complete
    spi0_slavecntrl(0); // pull cs high
    return S0SPDR;
}

/////////////////////slave stuff
void spi0_slaveinit()
{
    ISMASTER = 0;
    PINSEL0 &= ~S0PINSEL0_PINS; // cleared all bits for  uart0
    PINSEL0 |= 0x00005500;      // slave cs on 0.7

    // SPI0 rate may be calculated as: PCLK / SPCCR0 value
    //S0SPCCR = 0xA;   // Set SPI clock counter register - clock rate			         // sdo   0.6
    S0SPCR = 0X0000; // 0 for slave with default configs, control register ; master/slave select;  	// sd1 	  0.5

    VICIntSelect &= ~(1 << 10);
    VICVectCntl8 = 10 | (1 << 5);                       // set to spi0 & enable it
    VICVectAddr8 = (unsigned long int)spi0_slaveroutine; // sck	 0.4
	VICIntEnable |= 1 << 10;
    S0SPCR |= 0x80; // int enable on peripheral
}

char spi0_slavewrite(char data)
{

    S0SPDR = data;
    while ((S0SPSR & SPIFBIT) == 0);               // waitng for data transfer complete
    spi0_slavecntrl(0); // pull cs high
    return S0SPDR;      // use the data if required
}

char spi0_slaveread()
{
    while ((S0SPSR & SPIFBIT) == 1);
    return S0SPDR;
}

__irq void spi0_slaveroutine()
{   // wcol not considered
    __intupdate = 1;
    toggleLED(LED4);
		LED3_OFF();
		LED2_ON();
    RX = S0SPDR;
    SPI_BUFFER[BUFFERTAIL++]=RX;
    writeTHR(RX);
	S0SPINT=1; // clearing the interrupt bit
    VICVectAddr = 0x00;
}