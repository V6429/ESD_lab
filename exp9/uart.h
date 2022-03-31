#ifndef lpc214UART
#define lpc214UART

#include <LPC214x.h>
#define BAUDRATE1 96000
#define INTERRUPTIDENTIFICATIONMASK 0x0000000E

extern int _INTERUPDATE ;  // extern vars to be used on main
extern int BUFRINDX ;  // extern vars to be used on main
extern char __TX , __RX ;
extern char BUFFER[100];

// TODO AUTOBAUDRATE
//  move documentation to here

void inituart(int baudrate);// todo: baudrate implementation on this function

char readRBR();

void writeTHR(char transmitdata);

void uart_send_string(char *str);

void enableDLAB();

void disableDLAB();

void set_DLL_DLM_FDR(char dll, char dlm, char fdr);

void enableInterrupt(char intreg);

void enableFIFO();

void disableFIFO();

void cleanFIFOregs();

void setFIFOtriggerLevel(char triglevel);

void setLineCtrlReg();

char getlinestatus();

void enabletransmit();

void disabletransmit();

__irq void uart0routine();

#endif
