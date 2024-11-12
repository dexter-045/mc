#include<p18f4550.h>
#include<stdio.h>
#include "SPI_EEPROM.h"
#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0
#define LCDPORT LATB

void delay(unsigned int time)
{
 unsigned int i , j ;

    for(i = 0; i < time; i++)
    {
            for(j=0;j<100;j++);
    }
}


void SendInstruction(unsigned char command)
{
     LCD_RS = 0;		// RS low : Instruction
     LCDPORT = command;
     LCD_EN = 1;		// EN High
     delay(10);
     LCD_EN = 0;		// EN Low; command sampled at EN falling edge
     delay(10);
}

void SendData(unsigned char lcddata)
{
     LCD_RS = 1;		// RS HIGH : DATA
     LCDPORT = lcddata;
     LCD_EN = 1;		// EN High
     delay(10);
     LCD_EN = 0;		// EN Low; data sampled at EN falling edge
     delay(10);
}

void InitLCD(void)
{
    ADCON1 = 0x0F;
    TRISB = 0x00; //set data port as output
    TRISAbits.RA0 = 0; //RS pin
    TRISAbits.RA1 = 0; // EN pin

    SendInstruction(0x38);      //8 bit mode, 2 line,5x7 dots
    SendInstruction(0x06);	// entry mode
    SendInstruction(0x0C);	//Display ON cursor OFF
    SendInstruction(0x01);      //Clear display
    SendInstruction(0x80);      //set address to 0
}


void main()
{
    unsigned char text[16];         //temperory array for LCD
    unsigned char *string1 = "   SPI EEPROM";
    unsigned char write_data[10]={0x03,0x50,0x49,0xF6,0xAC,0x9B,0xDE,0x7A,0x6C,0x12};   //data to be written
    unsigned char read_data[10];            //array to hold data read from eeprom
    unsigned char ADDRESS = 0x0130;         //address of the memory location
    int i,j;                    //for counting

    InitLCD();
    while(*string1)             //display string1
        SendData(*string1++);
    delay(20000);


    InitSPI();                  //Initialise SPI
    SPI_CS = 0;
    SPI_Transmit(SPIEEPROM_WREN);   //set WRITE ENABLE
    SPI_CS = 1;

    delay(100);
        /*Write to EEPROM*/
    SPI_CS = 0;
    SPI_Transmit(SPIEEPROM_WRITE);  //WRITE instruction
    SPI_Transmit(ADDRESS >> 8);     //send the 16bit ADDRESS
    SPI_Transmit(ADDRESS & 0xFF);
    for(i=0;i<10;i++)               //send the write_data array to the EEPROM
        SPI_Transmit(write_data[i]);
    SPI_CS = 1;
    delay(100);

        /*Read from EEPROM*/
    SPI_CS = 0;
    SPI_Transmit(SPIEEPROM_READ);   //READ instruction
    SPI_Transmit(ADDRESS >> 8);     //send the 16bit ADDRESS
    SPI_Transmit(ADDRESS & 0xFF);
    for(i=0;i<10;i++)               //Read the data from EEPROM and store in read_data array
        read_data[i] = SPI_Receive();
    SPI_CS = 1;
    delay(100);

    SPI_CS = 0;             //this is an adjustment to turn off the buzzer

    SSPEN = 0;                      //disable SPI, RB0 and RB1 become GPIO pins
    InitLCD();                      //initialise SPI

    for(i=0;i<10;i++)               //compare the write_data and read_data arrays
    {
        SendInstruction(0x80);      //LCD 1st line
        sprintf(text,"write_data%d=0x%02X",i,write_data[i]);
        for(j=0;j<16;j++)
            SendData(text[j]);

        SendInstruction(0xC0);      //LCD 2nd line
        sprintf(text,"read_data%d =0x%02X",i,read_data[i]);
        for(j=0;j<16;j++)
            SendData(text[j]);
        delay(20000);
    }
    while(1);       //Stop here
}