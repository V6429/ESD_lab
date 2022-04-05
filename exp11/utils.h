
#ifndef utils_h
#define utils_h

#include <lpc214x.h>


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


void delay(int seconds);  // to stall main thread if required
void toggleLED(int led);  // toggles led on port1 
void replaceAll(char * str, char oldChar, char newChar);
void set_pll(void);
#endif