#include <lpc214x.h>
#include <math.h>
// macros
#define BIT(x) (1 << x)
#define SETTLINGWAIT_MILLISECS 100

// port-0

// on port1

// on port-0

void delayms(int millisseconds);

int main()
{
    int i = 0;
    PINSEL1 = 0x0080000; // Aout mode select	  p0.25 80000
    while (1)
    {

       for (i = 0; i <= 1023; i++)
       {
           DACR = i << 6;
           delayms(1);
       }
        
       for	(i=1023;i>=0;i--)
       {
           DACR = i << 6;
           delayms(1);
       }			

        DACR = 1023 << 6;
        delayms(250);
        DACR = 0 << 6;
        delayms(250);
        //20HZ
    }

    return 0;
}

void delayms(int millisseconds)
{
    /////////setuptimer
    T0CTCR = 0x00;         // setting the timer/counter peripheral to timer mode
    T0TCR = 0x02;          // reset the timer contents
    T0PR = 59999;          // prescale_value testing 1 milliseconds
    T0MR0 = millisseconds; // matching value
    // T0MCR = 0x0007;        //set mro    generate interrupt,reset TC,stop TC(for each mrreg)
    T0MCR = 0x06; // for mro donot generate interrupt,reset tc and stop tc at the end
    T0TCR = 0x01; // start the clock
    while (T0TC < millisseconds)
        ;
}