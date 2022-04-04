#include <LPC214x.h>	
#include <stdio.h>
#include "lcd.h"
//init e prom adress
#define EEPROM_Addr 0xA0		  	//device address	   //macros
#define I2Cwrite	0x00			//LSB bit 0 (write)
#define I2Cread		0x01			//LSB bit 1 (read)
//init control reg in lpc
#define I2C_ENABLE 	1 << 6		   //I2C Enable bit
#define I2C_START	1 << 5		   //Start Bit
#define I2C_STOP 	1 << 4		   //Stop Bit
#define I2C_SI		1 << 3		   //I2C interrupt flag
#define I2C_AACK  	1 << 2		   //assert ACK flag
// data for op
unsigned char write_array[4] = {1,2,3,4};
unsigned char read_array[4];
unsigned char write_str[4][6] = {"HAI","HELLO","HII","HELLO"};
unsigned char val[4];

 // INIT ENABLE
void I2CInit(void)
{
	PINSEL0 |= 0x00000050;							                       //P0.2 -> SCL0		P0.3 -> SDA0
	
	I2C0CONCLR  = I2C_ENABLE | I2C_START | I2C_STOP | I2C_SI | I2C_AACK;   //clear all the bits in CONTROL register
	
	//set I2C clock to work at 100Khz 
	I2C0SCLH = 0x4B ;							//set the high time of i2c clock; (15mhz / 100khz / 2) (duty cycle reg high half word)
	I2C0SCLL = 0x4B ;							//set the low time of i2c clock; (duty cycle reg low half word)
	
	I2C0CONSET = I2C_ENABLE ;					//enable the I2C Interface
}										

//start condition
void I2CStart(void)								  //Function to initiate a start condition on the I2C bus
{
unsigned int status;	
I2C0CONCLR = (I2C_START | I2C_STOP | I2C_SI | I2C_AACK); 	// clear all the bits in CONCLR register	
I2C0CONSET = (I2C_ENABLE );					 		  		//Enable the I2C interface 
I2C0CONSET = (I2C_START);					  				//set the STA bit 
while(!((status=I2C0CONSET)& I2C_SI));						//wait till interrupt flag becomes set // wait till i2c to start
}
 //condition for send
 void I2Csend(unsigned char data)
{	   
unsigned int status;
I2C0DAT = data;											// Assign data to register
I2C0CONCLR = I2C_START | I2C_STOP ;						// clear start bit for next operation 
I2C0CONCLR = I2C_SI;									// clear interrupt flag
while(!((status=I2C0CONSET)& I2C_SI));				   	//wait till interrupt flag becomes set
}
//condition for stop
void I2CStop(void)
{
unsigned int status;					 
I2C0CONCLR = I2C_START | I2C_SI | I2C_AACK;				    //clear all bits 
I2C0CONSET = I2C_STOP;										//set STOP bit
}




unsigned char I2Cget(void)
{
unsigned char data;
unsigned int status;

I2C0CONCLR = I2C_START | I2C_STOP;						// Clear start and stop bits
I2C0CONCLR = I2C_SI;									// clear interrupt flag				
I2C0CONSET = I2C_AACK;								   	// send ack to continue further data transfer
while(!((status=I2C0CONSET)& I2C_SI));					//wait till interrupt flag becomes set
data = I2C0DAT;											// Get data from register
return data;
}

//Make Delay
void makeDelay(int delimit)
{
/////////setuptimer									   
T0CTCR = 0x00; // setting the timer/counter peripheral to timer mode
T0TCR = 0x02; // reset the timer contents
T0PR = 59999; // prescale_value testing 1 milliseconds
T0MR0 = delimit; // matching value
T0MCR = 0x06; // for mro donot generate interrupt,reset tc and stop tc at the end
T0TCR = 0x01; // start the clock
while (T0TC < delimit)
;
}

int main()
{
unsigned int i;
LCDPrint("MASTER 2 SLAVE");
I2CInit();						  //initialize I2C
makeDelay(5000);

/* Write Sequence */ 

I2CStart();							//Assert START
I2Csend(EEPROM_Addr | I2Cwrite);	 //Device address with LSB bit 0  sending the adress of the ee prom
I2Csend(0x13);		   				//Address higher byte
I2Csend(0x49);						//Address lower byte
for(i=0;i<4;i++)
	I2Csend(write_array[i]); 		//write the array to EEPROM
I2CStop();				            //Assert STOP

//display written data
						
for(i=0;i<4;i++)
{
	sprintf(val,"%d)%s",write_array[i],write_str[i]);		  
    LCDPrint(val);
	makeDelay(3000);
}

/* Read Sequence */

LCDPrint("SLAVE 2 MASTER");
makeDelay(5000);

I2CStart();							 //Assert START
I2Csend(EEPROM_Addr | I2Cwrite);	 //Device address with LSB bit 0 (Dummy Write) //Points to the specific add in eeprom where the data is stored, in mastrer it is default to store the data in a spefic addr
I2Csend(0x13);						//Address higher byte
I2Csend(0x49);			 			//Address lower byte
I2CStart();							//Assert Restart
I2Csend(EEPROM_Addr | I2Cread);		//Device address with LSB bit 1
for(i=0;i<4;i++)
	read_array[i] = I2Cget(); 		//Read EEPROM
I2CStop();							//Assert STOP

/*Display data taken from slave*/

for(i=0;i<4;i++)
{
	sprintf(val,"%d)%s",read_array[i],write_str[i]);		  //display read data
	LCDPrint(val);
    makeDelay(3000);
}

while(1);	 			 //stop here forever
return 0;
}
