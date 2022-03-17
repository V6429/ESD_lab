#include <lpc214x.h>
/// TODO: express hex vals seperatly for readablity using |

// 10bit conversion time >= 2.44us

// port-0
#define TRIMPOT1 28 // AD01  //ADCONVERTER 0 input 1
#define TRIMPOT2 29 // AD02  //ADCONVERTER 0 input 1

// TODO: create a consistency in naming
// ADC
#define ADCREGISTER_DATA_MASK 0x0000FFC0
#define ADCREGISTER_OVRRUNBIT 0x40000000
#define ADCREGISTER_DONEBIT 0x80000000
#define ADCREGISTER_STARTCONVNOWMASK 0x07000000
#define ADCREGISTER_NOSTARTMASK 0x07000000

#include <lpc214x.h>
// useful stuff
// on port1
#define BIT(x) (1 << x)
#define LED1 16
#define LED2 17
#define LED3 18
#define LED4 19
#define LED1_MASK 0x00010000 // 00 01 00 00
#define LED2_MASK 0x00010000 // 00 02 00 00
#define LED3_MASK 0x00010000 // 00 04 00 00
#define LED4_MASK 0x00010000 // 00 08 00 00
#define BUZZER 11            // logic zer

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

// Macro Functions to turn ON LED
#define LED1_ON() IO1SET = (1 << 16)
#define LED2_ON() IO1SET = (1 << 17)
#define LED3_ON() IO1SET = (1 << 18)
#define LED4_ON() IO1SET = (1 << 19)
#define LED1_OFF() IO1CLR = (1 << 16)
#define LED2_OFF() IO1CLR = (1 << 17)
#define LED3_OFF() IO1CLR = (1 << 18)
#define LED4_OFF() IO1CLR = (1 << 19)

////////////////////////////SIDE NOTES//////////////////////////////////////////
// 1)
// Note: if the ADC is used, signal levels on analog input pins must not be above the
// level of V3A at any time. Otherwise, A/D converter readings will be invalid. If the A/D
// converter is not used in an application then the pins associated with A/D inputs can be
// used as 5 V tolerant digital IO pins
// 2)
// Warning: while the ADC pins are specified as 5 V tolerant (see Section 5.2 “Pin
// description for LPC2141/2/4/6/8” on page 52), the analog multiplexing in the ADC
// block is not. More than 3.3 V (VDDA) should not be applied to any pin that is selected
// as an ADC input, or the ADC reading will be incorrect. If for example AD0.0 and AD0.1
// are used as the ADC0 inputs and voltage on AD0.0 = 4.5 V while AD0.1 = 2.5 V, an
// excessive voltage on the AD0.0 can cause an incorrect reading of the AD0.1, although
// the AD0.1 input voltage is within the right range
////////////////////////////////////////////////////////////////////////////////////
// TODO:
//  ADCR   ---> The ADCR register must bewritten to select the operating mode before A/Dconversion can occur.
//  ADGDR  --->A/D Global Data Register. This register contains the ADC’s DONE bit and the result of the most recent A/D conversion
//  ADSTAT --->A/D Status Register. This register contains DONE and OVERRUN flags for all of the A/D channels, as well as the A/D interrupt flag.

/// qns
/// how to start
/// global or local()
int main()
{
    
    int i = 0;
    int adc_result;
    float actual_result;                                            // just for a temp delay
    PINSEL2 = 0x00000000;                                           // port 1-16,17,18,19 LED present
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4); // setting LEDS as output
    LED1_ON();
    for (i = 0; i < 90000; i++);
    LED2_ON();
    for (i = 0; i < 90000; i++);
    LED3_ON();
    for (i = 0; i < 90000; i++);
    LED4_ON();
    for (i = 0; i < 90000; i++);
    LED1_OFF();
    for (i = 0; i < 90000; i++);
    LED2_OFF();
    for (i = 0; i < 90000; i++);
    LED3_OFF();
    for (i = 0; i < 90000; i++);
    LED4_OFF();
    for (i = 0; i < 90000; i++);

    ///////////////////////////////////////////////////////////////////

    // setting pins of the trimpots to ADC functionality
    PINSEL1 = 0x05000000;   // setting pin 28 and 29 to adc functionality
    AD0CR = 0x00201402;        // 8bits to delect which channel,8dive for clockdiv(+1),
                             // burst enable bit,3 clks bit (also sets resolution) USED 000 FOR MAX HERE
                             // bit 21 is A/D converter is operational,bit 26:24 are start (000 for burst mode)
                             // bit 27 EDGE ( no use in burt mode)
    //IO0DIR &= (~0x30000000); // setting p0.28 & p0.30 as input ,we have inputs TODO:: exact pins
    //AD0CR = 0x00201403;      // select channels,clkdiv,burst or not,clks for burst,power down(for now),no start(type of start),Edge for  type of start
                             // refer data sheet comments  @ ADC location

    // waiting for the adc conversion (11 clock cycles on this mode ALTERNATIVE--->  use AD0INTEN)

    // recent conversion & done bits are here , channel identifier,overrun in burst mode
    // AD0GDR=

    // ADGSR ---- > global start register
    // ADGSR=

    // read AD0STAT register to get  statu
    // The A/D Status register allows checking the status of all A/D channels simultaneously.
    // The DONE and OVERRUN flags appearing in the ADDRn register for each A/D channel
    // are mirrored in ADSTAT. The interrupt flag (the logical OR of all DONE flags) is also found
    // in ADSTAT.
    //  simply all done and over run flags

    // AD0INTEN if you want to set an interrupt

    // global adc start register
    // ADGSR=

    // TODO
    //  READ THE ADGR REGISTER
    while (1)
    {
    
        LED4_ON();
		for (i = 0; i < 900000; i++);	  // why do we need this
        AD0CR |= BIT(24); // start
        while ((AD0DR1 & ADCREGISTER_DONEBIT)==0);
		AD0CR &= ~BIT(24); // stop
		LED4_OFF();


        adc_result = (AD0DR1 & ADCREGISTER_DATA_MASK);
        adc_result = adc_result>>6; // note
        actual_result = (adc_result * ((3.3 - 0) / 1023));
        LED1_ON();

        if (actual_result >= (3.3 / 4))
            LED1_ON();
        else
            LED1_OFF();

        if (actual_result >= (3.3 / 2))
            LED2_ON();
        else
            LED2_OFF();

        if (actual_result >= ((3.3 * 3) / 4))
            LED3_ON();
        else
            LED3_OFF();

        if (actual_result >= 3.3)
            LED4_ON();
        else
            LED4_OFF();

       AD0CR &= ~BIT(24); // stop


    }

    return 0;
}
