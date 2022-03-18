#include<lpc214x.h>


// what to do on uart0

int main()
{
    
    U0DLL=97; // got by calculation for 9600 (pclk is 15Mhz)
    U0DLM=0;  // baud rate hence will be 9600;
    U0FCR=0x0;




    return 0;
}
