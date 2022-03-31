#include <lpc214x.h>
#include "uart.h"
#include "utils.h"	
#include <string.h>
#include <stdlib.h>

// what to do on uart0
//**********************************************************************
int main()
{
    int i = 0;
    char transmitstring1[] = "Hello world\n";
    char transmitstring2[100];

    char tempstr[10];
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4);
    

    // U0DLL=97; // got by calculation for 9600 (pclk is 15Mhz)
    // U0DLM=0;  // baud rate hence will be 9600;
    // U0FCR=0x0;
    inituart(BAUDRATE1); // todo: baudrate implementation on this function
    while (1)
    {

        LED4_OFF();

        uart_send_string(transmitstring1);
		writeTHR(BUFFER[BUFRINDX - 1]);
        
        itoa(_INTERUPDATE,tempstr,10);
        strcpy(transmitstring2, "_INTERUPDATE=");
        strcat(transmitstring2,tempstr);
        strcat(transmitstring2,"\n");
        uart_send_string(transmitstring2)
        
        
        if (_INTERUPDATE == 1)
        {

            if (BUFFER[BUFRINDX - 1] == 'a')
                toggleLED(LED1);
            if (BUFFER[BUFRINDX - 1] == 'b')
                toggleLED(LED2);
            if (BUFFER[BUFRINDX - 1] == 'c')
                toggleLED(LED3);
        }
    }

return 0;
}
