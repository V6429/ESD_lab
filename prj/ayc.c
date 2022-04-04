#include <lpc214x.h>
#include "includes/uart.h"
#include "includes/utils.h"
#include <string.h>
#include <stdlib.h>
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
#define APBDIVVAL 0x00 // 00 div by 4  ,01 no div
#define AD0CR_SEL 0x00201402

void pwmsetup(void);

////////////////////////////SIDE NOTES//////////////////////////////////////////
// 1)
// Note: if the ADC is used, signal levels on analog input pins must not be above the
// level of V3A at any time. Otherwise, A/D converter readings will be invalid. If the A/D
// converter is not used in an application then the pins associated with A/D inputs can be
// used as 5 V tolerant digital IO pins
// 2)
// Warning: while the ADC pins are specified as 5 V tolerant (see Section 5.2 �Pin
// description for LPC2141/2/4/6/8� on page 52), the analog multiplexing in the ADC
// block is not. More than 3.3 V (VDDA) should not be applied to any pin that is selected
// as an ADC input, or the ADC reading will be incorrect. If for example AD0.0 and AD0.1
// are used as the ADC0 inputs and voltage on AD0.0 = 4.5 V while AD0.1 = 2.5 V, an
// excessive voltage on the AD0.0 can cause an incorrect reading of the AD0.1, although
// the AD0.1 input voltage is within the right range
////////////////////////////////////////////////////////////////////////////////////
// TODO:
//  ADCR   ---> The ADCR register must bewritten to select the operating mode before A/Dconversion can occur.
//  ADGDR  --->A/D Global Data Register. This register contains the ADC�s DONE bit and the result of the most recent A/D conversion
//  ADSTAT --->A/D Status Register. This register contains DONE and OVERRUN flags for all of the A/D channels, as well as the A/D interrupt flag.

/// qns
/// how to start
/// global or local()

int map(int x, int curr_min, int curr_max, unsigned int new_a, unsigned int new_b)
{

    //        (b-a)(x - min)
    // f(x) = --------------  + a
    //           max - min

    return (int)(((new_b - new_a) * (x - curr_min)) / ((float)curr_max - (float)curr_min)) + new_a;
}

int main()
{

    int i = 0;
    int adc_result1;
    unsigned long int adc_result2;
    float actual_result1, actual_result2;
    int pwm_value;
    char tempstringbffr[20];                                        // just for a temp delay
    PINSEL2 = 0x00000000;                                           // port 1-16,17,18,19 LED present
    IO1DIR = (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4); // setting LEDS as output
    LED1_ON();
    for (i = 0; i < 90000; i++)
        ;
    LED2_ON();
    for (i = 0; i < 90000; i++)
        ;
    LED3_ON();
    for (i = 0; i < 90000; i++)
        ;
    LED4_ON();
    for (i = 0; i < 90000; i++)
        ;
    LED1_OFF();
    for (i = 0; i < 90000; i++)
        ;
    LED2_OFF();
    for (i = 0; i < 90000; i++)
        ;
    LED3_OFF();
    for (i = 0; i < 90000; i++)
        ;
    LED4_OFF();
    for (i = 0; i < 90000; i++)
        ;

    ///////////////////////////////////////////////////////////////////

    // setting pins of the trimpots to ADC functionality
    PINSEL1 = 0x05000000; // setting pin 28 and 29 to adc functionality

    // P0.6/MOSI0/CAP0.2/AD1.0
    PINSEL0 |= 0x00003000;

    // 8bits to delect which channel,8dive for clockdiv(+1),
    // burst enable bit,3 clks bit (also sets resolution) USED 000 FOR MAX HERE
    // bit 21 is A/D converter is operational,bit 26:24 are start (000 for burst mode)
    // bit 27 EDGE ( no use in burt mode)
    // IO0DIR &= (~0x30000000); // setting p0.28 & p0.30 as input ,we have inputs TODO:: exact pins
    // AD0CR = 0x00201403;      // select channels,clkdiv,burst or not,clks for burst,power down(for now),no start(type of start),Edge for  type of start
    //  refer data sheet comments  @ ADC location

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
    pwmsetup();
    inituart(BAUDRATE1);
    set_pll();
    while (1)
    {
        AD0CR = AD0CR_SEL | BIT(1);
        AD0CR |= BIT(24); // start adc0
        while ((AD0DR1 & ADCREGISTER_DONEBIT) == 0)
            ;
        for (i = 0; i < 900; i++)
            ;
        adc_result1 = (AD0DR1 & ADCREGISTER_DATA_MASK);
        adc_result1 = adc_result1 >> 6; // note
        actual_result1 = (adc_result1 * ((3.3 - 0) / 1023.0));

        // P0.6/MOSI0/CAP0.2/AD1.0
        AD1CR = AD0CR_SEL | 1; // 0th bit((channel)) selection other things same as ad0
        AD1CR |= BIT(24);
        while ((AD1DR0 & ADCREGISTER_DONEBIT) == 0)
            ;
        for (i = 0; i < 900; i++)
            ;
        adc_result2 = (AD1DR0 & ADCREGISTER_DATA_MASK);
        adc_result2 = adc_result2 >> 6; // note
		pwm_value=	adc_result2;
	//	pwm_value=map(adc_result2,0,1023,0,(unsigned int)(0xffffffff-1));

       // actual_result2 = (adc_result2 * ((3.3 - 0) /(0xffffffff-1)));

        

        /// need to convert 10 it adc to 32 bit
        // 0 - 1023       0-(2^32-1)
        //

        //        (b-a)(x - min)
        // f(x) = --------------  + a
        //           max - min

        /*  uartSendString("\n_ADC VOLTAGE READING=");
          sprintf(tempstringbffr,"%d",(int)actual_result2); // alternative to atoi
          //strcat(tempstringbffr,"\n");
          uartSendString(tempstringbffr);  */

        if (actual_result1 <= 3.3 / 4)
        {
            LED1_ON();
            // PWMMR1=pwm_value;
            // PWMLER = BIT(1);
        }
        else
            LED1_OFF();

        if ((actual_result1 >= 3.3 / 4) && (actual_result1 <= 3.3 / 2 )   )
        {
            LED2_ON();
            // PWMMR2=pwm_value;
            // PWMLER = BIT(2);
        }
        else
            LED2_OFF();

        if ((actual_result1 >= 3.3 / 2 ) && (actual_result1 <= ((3.3*3) / 4 ) ))
        {
            LED3_ON();
            // PWMMR3=pwm_value;
            // PWMLER = BIT(3);
        }
        else
            LED3_OFF();

        if (actual_result1 >= ((3.3*3) / 4 ) )
        {
            LED4_ON();
            // PWMMR4=pwm_value;
            // PWMLER = BIT(4);
        }
        else
            LED4_OFF();
    }

    return 0;
}

void pwmsetup(void)
{
    PINSEL0 = 0x000A800A; // choosimg PWM5 in pin 0.9
    // PINSEL0 = (PINSEL0 & ~(1 << 16)) | (1 << 17);
    PWMPCR = 0x0;   // single edged ( default 0)
    PWMPR = 60 - 1; // Prescale resolution
    PWMMR0 = 5000;  // Cycle duration (Ton + Toff)
    PWMMR0 = 5000;
    PWMMR1 = 250;
    PWMMR2 = 250;
    PWMMR3 = 250;
    PWMMR4 = 250;
    PWMMR6 = 250; // Ton duration
    // IO1DIR = BIT(0) ;

    PWMMCR = BIT(1); // interupt, reset, stop.. (here only reset) for MMR0

    PWMLER = BIT(6) | BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4); // For updating match register from shadow register

    PWMPCR = BIT(14) | BIT(9) | BIT(10) | BIT(11) | BIT(12); // 6:2 single or double edge, 14:9 , enable or disable output

    PWMTCR = BIT(1);
    PWMTCR = BIT(0) | BIT(3); // PE; PWM enable
    // CR; reset
    // CE: timer and prescalar enabled
}

