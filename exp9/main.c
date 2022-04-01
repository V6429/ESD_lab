#include <lpc214x.h>
#include "uart.h" // lib for uart0
#include "utils.h" // some custom functions
#include <string.h>
#include <stdlib.h>//atoi alternative 
#define PRINTBUFFER 0 // set 1 or 0
#define APBDIVVAL 0x00  // 00 div by 4  ,01 no div 

// what to do on uart0
//**********************************************************************
// use the initfunction
// use the sendstring function
// recived charachters are loaded in a buffer through interrupt
void set_pll(void);
int main()
{
    int i = 0;
    char transmitstring1[] = "Hello world\n";
    char tempstringbffr[20];
    char tempstr[10];
	set_pll();
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
			  if(strstr(BUFFER,"buffer")!=NULL){
		uartSendString("BUFFER=");
		replaceAll(BUFFER,'\n','#');        
        uartSendString(BUFFER); // prints the entire buffer
		uartSendString("\n");
		memset(BUFFER,'0',100);	// clear buffer
        BUFRTAIL=0;
		BUFFER[BUFRTAIL]='\0';
        

         // for neet display


            }

        
        }
    }

    return 0;
}


void set_pll(void)
{
  PLL0CON=0x01;             // PPLE=1 & PPLC=0 so it will be enabled  but not connected after FEED sequence
  PLL0CFG=0x24;          // set the multipler to 5 (i.e actually 4) i.e 12x5 = 60 Mhz (M - 1 = 4), Set P=2 since we want FCCO in range So , Assign PSEL =01 in PLL0CFG as per the table.
  PLL0FEED=0XAA;          //feed
  PLL0FEED=0X55;
  while((PLL0STAT&(1<<10))==0);      // check whether PLL has locked on to the  desired freq by reading the lock bit in the PPL0STAT register
  PLL0CON=0x03;          //enable & connect pll
  PLL0FEED=0XAA;
  PLL0FEED=0X55;
  VPBDIV = APBDIVVAL;          // PCLK is same as CCLK i.e 60Mhz	when 01
}
