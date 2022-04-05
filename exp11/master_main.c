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
        uartSendString("\nG transmitted flush=");
        writeTHR(flush);
        delay(200);
        flush = spi0_masterwrite('A');
        uartSendString("\nA transmitted flush=");
        writeTHR(flush);
        delay(200);

        flush = spi0_masterwrite('M');
        uartSendString("\nM transmitted flush=");
        writeTHR(flush);
        delay(200);

        flush = spi0_masterwrite('E');
        uartSendString("\nE transmitted flush=");
        writeTHR(flush);
        delay(200);

        flush = spi0_masterwrite('D');
        uartSendString("\nD command flush=");
        writeTHR(flush);
        delay(200);

        uartSendString("\nread=");
        writeTHR(spi0_masterread());
        LED4_ON();
        delay(200);
        /* code */
    }

    return 0;
}