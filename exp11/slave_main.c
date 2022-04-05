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

        if (__intupdate)
        {
            __intupdate = 0;
            writeTHR(RX);
            uartSendString("\n");
            SPI_BUFFER[100] = '\0';

            if (RX == 'D')
            {   spi0_slavewrite('F');
                uartSendString("command D received: sending 'F' back\n");
            }
        }

        LED3_ON();
        LED2_OFF();
        /* code */
    }

    return 0;
}