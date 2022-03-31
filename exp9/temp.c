#include <lpc214x.h> 0+ char data[] = {'H','a','i',' ', 'S','r','e','e','r','a','g',' ','&','S','u','b','b','i','a','h', ' .','.','.' };// Function to initialise UART
int main(){ int i=0;
char Recieved_Data;
PINSEL0 |= 0x05;
// U0FCR |= 0x03; // FIFO reset and enable
U0LCR |= 0x83; // 8 bit no parity DLAB set 1 for Baud rate calculation
U0DLL |= 97;//110;
// U0DLM |= 0x01;//1;
// U0FDR = 0xf1; //To set MULVAL and DIVADDVAL
U0LCR = 0x03; //SeT DLAB to 0 to lock MULVAL and DIVADDVAL
while(1)
{
for(i=0;i<20;i++)
{
while( (U0LSR & 0x01) == 0);
Recieved_Data = U0RBR;
while(!(U0LSR & (1<<5)));
U0THR = Recieved_Data; }
}
}
