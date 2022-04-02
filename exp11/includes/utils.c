
#include "utils.h"
#include <lpc214x.h>

void delay(int seconds)
{
    /////////setuptimer
    T0CTCR = 0x00;          // setting the timer/counter peripheral to timer mode
    T0TCR = 0x02;           // reset the timer contents
    T0PR = 59999;           // prescale_value testing 1 milliseconds
    T0MR0 = seconds * 1000; // matching value
    T0MCR = 0x06;           // for mro donot generate interrupt,reset tc and stop tc at the end
    T0TCR = 0x01;           // start the clock
    while (T0TC < seconds)
        ;
}

void toggleLED(int led)
{

    if (IOPIN1 & (1 << led)) // if on
        IO1CLR = 1 << led;   // turn off
    else                     // turn on
        IO1SET = 1 << led;
}


void replaceAll(char * str, char oldChar, char newChar)
{
    // src
    // https://codeforwin.org/2016/04/c-program-to-replace-all-occurrences-of-character-in-string.html
    int i = 0;

    /* Run till end of string */
    while(str[i] != '\0')
    {
        /* If occurrence of character is found */
        if(str[i] == oldChar)
        {
            str[i] = newChar;
        }

        i++;
    }
}

//  todo
void set_pll()
{
  PLL0CON=0x01;             // PPLE=1 & PPLC=0 so it will be enabled  but not connected after FEED sequence
  PLL0CFG=0x24;          // set the multipler to 5 (i.e actually 4) i.e 12x5 = 60 Mhz (M - 1 = 4), Set P=2 since we want FCCO in range So , Assign PSEL =01 in PLL0CFG as per the table.
  PLL0FEED=0XAA;          //feed
  PLL0FEED=0X55;
  while((PLL0STAT&(1<<10))==0);      // check whether PLL has locked on to the  desired freq by reading the lock bit in the PPL0STAT register
  PLL0CON=0x03;          //enable & connect pll
  PLL0FEED=0XAA;
  PLL0FEED=0X55;
  VPBDIV = 0x00;          // PCLK is same as CCLK i.e 60Mhz	when 01
}
