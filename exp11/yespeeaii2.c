#include <lpc214x.h> 
  
void spi_setup(void) ;
char SPI_Read(void);


  void main() 
  {
     int data;
     spi_setup();
	 PINSEL2=0x00;	
	 IO1DIR=0X00;
	 data =  SPI_Read();
	 IO1SET=	data << 16;
  
 // interrupt_init(); 

  }

	void spi_setup() 
	{
	 PINSEL0 = 0x00001500;									                                      //cs	0.7
  	 S0SPCCR= 0X10;// Set SPI clock counter register - clock rate			                 // sdo   0.6
	 S0SPCR = 0X0000;  // control register ; master/slave select; hardware interruopt enable 	// sd1 	  0.5
								                                                           // sck	 0.4
	  // Read SPI status 
	  // SPI data (received) 
	}

char SPI_Read()
{
 //	IO0CLR = (1<<7);  /* SSEL = 0, enable SPI communication with slave */
	S0SPDR = 0xFF;  /* Transmit Flush byte */
	while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed */
//	IO0SET = (1<<7);  /* SSEL = 1, disable SPI communication with slave */
	return S0SPDR;  /* Return the data received */
}
