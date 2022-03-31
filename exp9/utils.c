
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