#include<p18f4550.h>
#include<stdio.h>
#include "SPI_EEPROM.h"

void delay1(unsigned int time)
{
 unsigned int i,j;
    for(i=0;i<time;i++)
    {
        for(j=0;j<100;j++);
    }
}

void InitSPI()
{
    ADCON1 = 0x0f;
    TRISBbits.TRISB0 = 1;   //SDI as input
    TRISBbits.TRISB1 = 0;   //SCK as output
    TRISCbits.TRISC7 = 0;   //SDO as output

    TRISAbits.TRISA5 = 0;   //CS as output
    TRISEbits.RE1 = 0;      //HOLD Pin

    SPI_CS = 1;
    EEPROM_HOLD = 1;

    SSPCON1bits.SSPEN = 0;
    
    SSPSTATbits.SMP = 0;    //set data to be sampled at the middle
    SSPSTATbits.CKE = 1;    //Transmition occurs on transition from active to Idle clock state

    SSPCON1bits.CKP   = 0;    //idle state of clock is LOW
    SSPCON1bits.SSPM0 = 0;    //spi clock is Fosc/64
    SSPCON1bits.SSPM1 = 1;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM3 = 0;

    SSPCON1bits.SSPEN = 1;  //Enable the SPI Interface
}

void SPI_Transmit(unsigned char Data)
{
    PIR1bits.SSPIF = 0;
    SSPBUF = Data;     
    while(PIR1bits.SSPIF == 0);     //wait till transmission activity is over
    PIR1bits.SSPIF = 0;         //clear the interrupt flag
    Data = SSPBUF;              //empty the SSPBUF register
}

unsigned char SPI_Receive()
{
    unsigned char RecData;
    SSPBUF = 0x00;              //send a dummy data
    while(PIR1bits.SSPIF == 0);     //wait till transmission/reception is complete
    RecData = SSPBUF;
    PIR1bits.SSPIF = 0;
    return RecData;
}