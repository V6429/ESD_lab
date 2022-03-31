 #include <lpc214x.h>
 #include <stdio.h>
 #include <string.h>


 __irq void uart1_interrupt(void);

 void init_uart1(void);                     //////initialize uart1
 void uart1_write(char data);           //////send char on uart1
 void uart1_send_string(char *str);//////send string on uart1

 void Uart0_string(char *str);          //////send string on uart0
 void U0write(char data);                   //////send 

 void init_uart0(void);                     //////initialize uart0

 void pll_init(void);
 void timer_init(void);
 void delay_ms(unsigned int ms);

 //void clear_data();                               /////clear buffer

 char buff[160];
 //char sms_receive[60];
 int i, a = 0;

 int main(void)
 {
    pll_init();
    timer_init();
    init_uart0();
    //init_uart1();


    VICVectAddr0 = (unsigned) uart1_interrupt;   /////attach interrupt routine
    VICVectCntl0 = (1<<5)|7;                                                                    ////enable uart irq slot for uart1
    VICIntEnable |= (1<<7); ////enable uart1 interrupt 
    VICIntSelect = 0x00000000;
    init_uart1();

    Uart0_string("gsm test\r\n");


    delay_ms(3000);

    while(1)
    {
        uart1_send_string("AT\r\n");
        delay_ms(400);
        if(strstr(buff,"OK"))
        {
            Uart0_string(buff);
        }

        else
        {
            Uart0_string("ERROR ");
        }

        delay_ms(3000);

        uart1_send_string("AT+CNMI=?\r\n");
        delay_ms(400);
        if(strstr(buff,"+CNMI"))
        {
            Uart0_string(buff);
        }

        else
        {
                Uart0_string("ERROR ");
        }

        delay_ms(3000);
        //clear_data();
        memset(buff, 0 , sizeof(buff)); 
    }
  }


 /*void clear_data()
 {
    for(i = 0; i<=sizeof(buff); i++)
    {
        buff[i] = 0;
    }
 }*/

 __irq void uart1_interrupt(void)
 {
    buff[a] = U1RBR;
    a++;
    VICVectAddr = 0x00;
 }

 void init_uart1(void)
 {
    PINSEL0 = PINSEL0 | 0x00050000;    ////UART1 pin selection

    U1LCR = 0x83;            ////8 bit, no parity, 1 stop bit, DLAB - 1
    U1DLL = 0x56;                ////86 in decimal
    U1DLM = 0x00;
    U1FDR = 0xF2;

    U1LCR &= 0x0f;          /////setting DLAB bit to 0

    U1IER = 0x01;               ////enable interrupt
 }

 void uart1_write(char data)
 {
    U1IER = 0x00;
    while(!(U1LSR &(1<<5)));        ////U1LSR contains THRE status bit, wait while THR is empty
    U1THR = data;
    U1IER = 0x01;
 }

 void uart1_send_string(char *str)
 {
    U1IER = 0x00;
    while(*str != '\0')
    {
        uart1_write(*str++);
    }
    U1IER = 0x01;
 }


 /*char uart1_receive()
 {
    while(!(U1LSR & (1<<0)));
    return U1RBR;
 }*/

 void init_uart0()
 {
    PINSEL0 = 0x00000005;   ////selects TxD P0.0 and RxD P0.1

    U0LCR = 0x83;           ////8 bit, no parity 1 stop bit, DLAB = 1
    U0DLL = 0x56;     //// 86 in decimal
    U0DLM = 0x00;
    U0FDR = 0xF2;           ////MULVAL and DIVADDVAL values

    U0LCR &= 0x0F;
 }

 void U0write(char data)
 {
    while(!(U0LSR & (1<<5)));  ///////wait till THR is not empty
    U0THR = data;                            /////// write data to Transmit in U0THR

 }

 void Uart0_string(char *str)
 {
    while(*str != '\0')
    {
        U0write(*str++);
    }
 }

 void pll_init(void)
 {
    PLL0CON = 0x01;        /////enable PLL////
    PLL0CFG = 0x24;              /////configure PLL/////
    PLL0FEED = 0xAA;             //////feed sequence/////
    PLL0FEED = 0x55;             //////feed sequence/////

    while(!(PLL0STAT & 0x00000400)); ///is locked////

    PLL0CON = 0x03;     ////connect PLL////
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;

    //VPBDIV = 0x01; /////// PCLK = CCLK = 60MHz
    VPBDIV = 0x00;   /////// PCLK = 60/4 = 15MHz
 }

 void timer_init(void)
 {
    //T0TCR = 0x00;
    T0CTCR = 0x0;       ////selection of timer mode with every rising pclk edge
    T0PR = 14999;           ////prescale register value will generate 1 ms at 60MHz
    T0TCR = 0x02;     ////reset the timer counter and prescale couter
 }

 void delay_ms(unsigned int ms)
 {
    //T0TC = 0x00000000;
    T0TCR = 0x02;     /////reset timer. Timer should be reset
    T0TCR = 0x01;     /////enable timer to generate delay
    T0MR0 = ms;
    while(T0TC < T0MR0); /////wait until Timer counter register reaches desired delay
    T0TCR = 0x00;     /////disable timer

 }