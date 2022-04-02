#include <LPC214x.h>
#include "includes/uart.h"
#include "includes/utils.h"

int main()
{   
    setpll();
    spi0_masterinit();
    while (1)
    {
        LED4_OFF();


        /* code */
    }

    return 0;
}