#include <LPC214x.h>
#include "uart.h"
#include "utils.h"
#include "spi0.h"
int main()
{
    char flush;
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4);
    inituart(BAUDRATE1);
    set_pll();
    spi0_slaveinit();
    while (1)
    {
        SPI_BUFFER[100] = '\0';
        
        delay(5);
        uartSendString("\nbuffer=");
		uartSendString(SPI_BUFFER);
		 //writeTHR(RX);
        //spi0_slavewrite('b');
	//	writeTHR(spi0_slaveread());
        LED3_ON();
        /* code */
    }

    return 0;
}