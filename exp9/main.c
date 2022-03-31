#include <lpc214x.h>
#include "uart.h" // lib for uart0
#include "utils.h" // some custom functions
#include <string.h>
#include <stdlib.h>//atoi alternative 
#define PRINTBUFFER 0 // set 1 or 0

// what to do on uart0
//**********************************************************************
// use the initfunction
// use the sendstring function
// recived charachters are loaded in a buffer through interrupt
int main()
{
    int i = 0;
    char transmitstring1[] = "Hello world\n";
    char tempstringbffr[20];
    char tempstr[10];
	BUFFER[0]='\0';		 // an extern buffer

    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4);

    inituart(BAUDRATE1); // todo: baudrate implementation on this function
    for (i = 0; i < 10; i++)
    {
        uartSendString(".\n");
        delay(4);
    }


    uartSendString(transmitstring1);
    BUFFER[100]='\0'; // for test
  

    while (1)
    {

        LED4_OFF();//  used this for identifing on main 
        if(PRINTBUFFER){
		uartSendString("BUFFER=");        
        uartSendString(BUFFER); // prints the entire buffer
        uartSendString("\n");
        replaceAll(BUFFER,'\n','#'); // for neet display
	   }
       
        //example of putting something to debug
        // uartSendString("\n_INTERUPDATE=");
        // sprintf(tempstringbffr,"%d",_INTERUPDATE); // alternative to atoi
        // strcat(tempstringbffr,"\n");
        // uartSendString(tempstringbffr);


        if (_INTERUPDATE == 1)
        {
                 // parse the buffer content as per requirment 
            if (BUFFER[BUFRTAIL - 2] == 'a')
                toggleLED(LED1);
            if (BUFFER[BUFRTAIL - 2] == 'b')
                toggleLED(LED2);
            if (BUFFER[BUFRTAIL - 2] == 'c')
                toggleLED(LED3);
                _INTERUPDATE=0;

            // the following is a bad implementation
            if(strstr(BUFFER,"whoami")!=NULL){
                BUFRTAIL=0;
                memset(BUFFER,'0',100);
                BUFFER[BUFRTAIL]='\0';
                uartSendString("LPC2148: a dump machine . (•_•) ( •_•)>⌐■-■ (⌐■_■) \n");
            }

            if(strstr(BUFFER,"hello")!=NULL){
                BUFRTAIL=0;
                memset(BUFFER,'0',100);
                BUFFER[BUFRTAIL]='\0';
                uartSendString("LPC2148: WORLD 	(ノ ゜Д゜)ノ ︵ ┻━┻\n");
            }
        
        }
    }

    return 0;
}
