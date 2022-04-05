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
    spi0_masterinit();


    
	while (1)
    {
    
        flush = spi0_masterwrite('G');
        delay(15);
        uartSendString("\nflush=");
        writeTHR(flush);
        LED4_ON();
        /* code */
    }

    return 0;
}