#include <lpc214x.h>
// useful stuff
#define BIT(x) (1 << x)
// on port1
#define LED1 16
#define LED2 17
#define LED3 18
#define LED4 19
#define LED1_MASK 0x00010000 //00 01 00 00
#define LED2_MASK 0x00010000 //00 02 00 00
#define LED3_MASK 0x00010000 //00 04 00 00
#define LED4_MASK 0x00010000 //00 08 00 00

// on port-0
#define SWITCH1 15
#define SWITCH2 13
#define SWITCH3 12
#define SWITCH4 30
#define INTERRUPTPIN 30
#define SWITCH1_MASK 0x00008000
#define SWITCH2_MASK 0x00002000
#define SWITCH3_MASK 0x00001000
#define SWITCH4_MASK 0x40000000

//Macro Functions to turn ON LEDs
#define LED1_ON() IO1SET = (1 << 16)
#define LED2_ON() IO1SET = (1 << 17)
#define LED3_ON() IO1SET = (1 << 18)
#define LED4_ON() IO1SET = (1 << 19)
//Macro Functions to turn OFF LEDs
#define LED1_OFF() IO1CLR = (1 << 16)
#define LED2_OFF() IO1CLR = (1 << 17)
#define LED3_OFF() IO1CLR = (1 << 18)
#define LED4_OFF() IO1CLR = (1 << 19)

//Timer0 related
//on interrupt register
#define TIMER0MR0BIT 0x00000001
#define TIMER0MR1BIT 0x00000002
#define TIMER0MR2BIT 0x00000004
#define TIMER0MR3BIT 0x00000008

int CONTINUEMAINTHREAD = 0; //to stall the main thread
__irq void timerroutine();
void delayms(int millisseconds);
void delay(int seconds);
void setupexternalinterrupt();
__irq void externalinterruptfunction();

int main()
{
    unsigned int inputportstate, outputportstate;
    //setting pinouts
    // setting all pins on port to  GPIO function
    // TODO : use exact pin numbers
    PINSEL0 = 0x00000000; // port 0-12,13,15 switches present
    PINSEL1 = 0x20000000; // port 0-30 switch present configured as EINT3
    PINSEL2 = 0x00000000; // port 1-16,17,18,19 LED present
    // setting LEDS as output
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4);
    //setting switches as input
    IO0DIR = 0x00000000; // by default it will be zero anyway

    // setup the interupt for TIMER
    VICIntSelect &= ~BIT(4); //VICIntSelect by default 4th bit is 0 representing IRQ
    VICVectCntl5 = 0x24;     // this is a timer0 interrupt
    VICVectAddr5 = (unsigned long int)timerroutine;
    VICIntEnable = BIT(4);

    //external interup on sitch3 example
    setupexternalinterrupt();

    while (1)
    { /// main looop

        // read the pins for switches
        inputportstate = IOPIN0;
        outputportstate = IOPIN1;

        LED4_ON();
        delay(1);
        while (!CONTINUEMAINTHREAD)
            ;
        LED4_OFF();
        delay(1);
        while (!CONTINUEMAINTHREAD)
            ;
        // GPIO example
        if (((inputportstate & SWITCH3_MASK) == 0))
        { // if switch pressed,pulled to zero

            // LED3_ON();
            if (outputportstate & LED1_MASK) // if led on
            {
                LED1_OFF();
                delay(1);
                while (!CONTINUEMAINTHREAD)
                    ;
            }
            else
            {
                LED1_ON(); //turn on
                delay(1);
                while (!CONTINUEMAINTHREAD)
                    ;
            }
        }

        LED3_OFF();
    }

    return 0;
}

__irq void timerroutine()
{
    int timerinterruptregister = T0IR;
    CONTINUEMAINTHREAD = 1;
    T0IR = timerinterruptregister;
    VICVectAddr = 1; //write dummy value onto the addr after routine is completed
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

void delay(int seconds)
{
    /////////setuptimer
    T0CTCR = 0x00;          // setting the timer/counter peripheral to timer mode
    T0TCR = 0x02;           // reset the timer contents
    T0PR = 59999;           // prescale_value testing 1 milliseconds
    T0MR0 = seconds * 1000; // matching value
    T0MCR = 0x06;           // for mro donot generate interrupt,reset tc and stop tc at the end
    T0TCR = 0x01;           // start the clock
    while (T0TC < millisseconds);
        ;
}

void setupexternalinterrupt()
{

    // INTERRRUPT SETUP
    EXTMODE = BIT(3);           //setting for edge triggering for ext3
    EXTPOLAR = 0;               // falling edge
    VICIntSelect &= ~BIT(17);   //VICIntSelect by default 17th bit is 0 representing IRQ
    VICVectCntl10 = 0x00000031; // bits0:4 says EXTINT3 bit5 1 says enable
    VICVectAddr10 = (unsigned long int)externalinterruptfunction;
    VICIntEnable |= BIT(17); // enablint EXTINT3
}

__irq void externalinterruptfunction()
{
    int blinkcount = 5;
    for (; blinkcount != 0; blinkcount--)
    {
        LED4_ON();
        delay(50);
        while (!CONTINUEMAINTHREAD)
            ;
        LED4_OFF();
        delay(50);
        while (!CONTINUEMAINTHREAD)
            ;
    }

    /// completed rotine
    EXTINT = BIT(3);
    VICVectAddr = 1;
}
