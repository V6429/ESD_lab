#include <lpc214x.h>
// TODO:REMOVE HARCODED VALUES

// useful stuff
#define BIT(x) (1 << x)
// on port1
#define LED1 16
#define LED2 17
#define LED3 18
#define LED4 19
#define LED1_MASK 0x00010000 // 00 01 00 00
#define LED2_MASK 0x00010000 // 00 02 00 00
#define LED3_MASK 0x00010000 // 00 04 00 00
#define LED4_MASK 0x00010000 // 00 08 00 00

// Macro Functions to turn ON LEDs
#define LED1_ON() IO1SET = (1 << 16)
#define LED2_ON() IO1SET = (1 << 17)
#define LED3_ON() IO1SET = (1 << 18)
#define LED4_ON() IO1SET = (1 << 19)
// Macro Functions to turn OFF LEDs
#define LED1_OFF() IO1CLR = (1 << 16)
#define LED2_OFF() IO1CLR = (1 << 17)
#define LED3_OFF() IO1CLR = (1 << 18)
#define LED4_OFF() IO1CLR = (1 << 19)

// Timer0 related
// on interrupt register
#define TIMERMR0BIT 0x00000001
#define TIMERMR1BIT 0x00000002
#define TIMERMR2BIT 0x00000004
#define TIMERMR3BIT 0x00000008

///////////////////////////////////////////////////
int GLOBAL_PULSECOUNT = 0; // used to share info between pulse starter and irq
int GLOBAL_NOOFTIMERRUNS = 0;
int GLOBAL_TIMERLASTRUNTIME = 0;
int GLOBAL_TIMERLASTRUN = 0;
int GLOBAL_DELAY=0;

void initpulsing_led();
void startpulsingled(int blink_nooftimes, unsigned int delay);
__irq void ledpulsingroutine();

////////////////////////////////////////////////////////////

void delayms(int millisseconds); // to stall main thread if required
void delay(int seconds);  // to stall main thread if required
void toggleLED(int led);  // toggles led on port1 



int main()
{	int i=0;
	IO0SET = 1 << 11;		// turn buzzer off
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4);
    initpulsing_led();
    startpulsingled(-1,100);		// -1 for infinite blinks


    while (1)
    { 
		toggleLED(LED1);
		for(i=0;i<60000;i++);
		
    }

    return 0;
}

void initpulsing_led()
{

    // this function configures led pulsing
    PINSEL2 = 0x00000000; //todo exact pin
    IO1DIR |= (1 << LED4);

    // configure timer
    T1CTCR = 0x00;      // setting the timer/counter peripheral to timer mode
    T1TCR = 0x02;       // reset the timer contents
    T1PR = 0;           // prescale_value  keeping it zero 
    // T1MR0 = 0xdeadbeef; // dummy
    T1MCR = 0x00000003; //   generate interrupt,reset TC on match,donot stop TC

    // configure interrupt
    VICIntSelect &= ~BIT(5); // VICIntSelect by default 4th bit is 0 representing IRQ
    VICVectCntl11 = 0x25;    // this is a enabled,timer1 interrupt
    VICVectAddr11 = (unsigned long int)ledpulsingroutine;
}

void startpulsingled(int blink_nooftimes, unsigned int delay)
{
    // this function initializes  global values
    GLOBAL_PULSECOUNT = blink_nooftimes;
    GLOBAL_DELAY=delay;


    if (delay - 71 <= 0)
    {
        GLOBAL_NOOFTIMERRUNS = 0;
        GLOBAL_TIMERLASTRUN = 1;
    }
    else
    {
        // GLOBAL_NOOFTIMERRUNS=delay/71.582878825;  /// actual  timer 71.56
        // 2^32 = 4,294,967,295
        // 60 000 000
        // ----->71.58278825
        // ----->reminder is 34967295 cycles  (the 0.582878825 part)
        // enter the adjustment cycles
        // if we are using the FFFF FFFF value on complete runs
        // we will have serous trouble
        // lot of clock cycles can get miscalculated & lost in float to int or int to float conversion
        // this error increases leniarly with no of runs
        // so sticking to MR value
        GLOBAL_NOOFTIMERRUNS = delay / 71;
        GLOBAL_TIMERLASTRUNTIME = delay % 71;
    }
    VICIntEnable |= BIT(5);//enable interrupt for timer1

    if (!GLOBAL_NOOFTIMERRUNS) // if a single run set match register appropriatly
        T1MR0 = 60000000 * delay;	
    else
    {
        T1MR0 =0xFDEA70FF; //0xFDEA7100-1;  60Mhz x 71sec // max value 4260000000 at 71 seconds
        //2158EFF
        GLOBAL_NOOFTIMERRUNS--;
    }

        // start the clock

        T1TCR = 0x01;

    // 4294967295 / 60000000
}

__irq void ledpulsingroutine()     
{   
    toggleLED(LED2);
    // looks for global variables initialized using  startpulsingled   

    // at the end of THE LAST RUN
    if (GLOBAL_TIMERLASTRUN && (GLOBAL_NOOFTIMERRUNS == 0))
    {
        // toggle the led   
        toggleLED(LED4);

        //repeat the entire process
        if(GLOBAL_PULSECOUNT !=-1)
        GLOBAL_PULSECOUNT--;
 
        if(GLOBAL_PULSECOUNT >0)
        {GLOBAL_PULSECOUNT--;
        startpulsingled(GLOBAL_PULSECOUNT,GLOBAL_DELAY);}
        

    }



    // if this not the lastrun 
    // but last time of run
    if ( GLOBAL_NOOFTIMERRUNS == 0 && !GLOBAL_TIMERLASTRUN)
    {
        // we also need the timer to end on its last run 
        T1MCR = 0x00000007;              //  enable interrupts ,reset tc ,stop tc
        T1MR0 = 60000000*GLOBAL_TIMERLASTRUNTIME; // the last run of each set
        GLOBAL_TIMERLASTRUN = 1;
    }
    else if ( GLOBAL_NOOFTIMERRUNS != 0 && !GLOBAL_TIMERLASTRUN)  // this is one of the run in the set
    {
        // MR0 uses the same previus stuff
        GLOBAL_NOOFTIMERRUNS--;   // tc is configured to run 
        GLOBAL_TIMERLASTRUN = 0;
    }

    // clearing the interrupt
    toggleLED(LED2);
    // reset timer interrupt setting
    T1IR = TIMERMR0BIT; // resetting interupt on channel MR0
    // reser nvic interrupt
    VICVectAddr = 1;
    // as per the data sheet
}

void togglebuzzer()
{
	if (((IOPIN1) && 1 << 11)==0)// if logic 0 // buzzer turns on on logic zero
		IOCLR0=1<<11;
	else 
		IO0CLR = 1 << 11;

}


void toggleLED(int led)
{

    if ((IOPIN1) && 1 << led) // if on turn off
        IO1SET = 1 << led;
    else // turn on       
		IO1CLR = 1 << led;
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
    while (T0TC < seconds);
        ;
}
