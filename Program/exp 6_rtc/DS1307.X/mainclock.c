#include <p18f4550.h>
#include <stdio.h>
#include "i2c.h"

unsigned char str[16];

#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0
#define LCDPORT LATB

void lcd_delay(unsigned int time)
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
     lcd_delay(10);
     LCD_EN = 0;		// EN Low; command sampled at EN falling edge
     lcd_delay(10);
}

void SendData(unsigned char lcddata)
{
     LCD_RS = 1;		// RS HIGH : DATA
     LCDPORT = lcddata;
     LCD_EN = 1;		// EN High
     lcd_delay(10);
     LCD_EN = 0;		// EN Low; data sampled at EN falling edge
     lcd_delay(10);
}

void InitLCD(void)
{
    ADCON1 = 0x0F;
    TRISB = 0x00; //set data port as output
    TRISAbits.RA0 = 0; //RS pin
    TRISAbits.RA1 = 0; // EN pin

    SendInstruction(0x38);      //8 bit mode, 2 line,5x7 dots
    SendInstruction(0x06);	//entry mode
    SendInstruction(0x0C);	//Display ON cursor OFF
    SendInstruction(0x01);      //Clear display
    SendInstruction(0x80);      //set address to 0
}


unsigned char get_reg(unsigned char address)     // Gets time
{
    unsigned char data;
    i2c_start();
    i2c_write(0b11010000);
    i2c_write(address);
    i2c_restart();
    i2c_write(0b11010001);
    SSPCON2bits.ACKDT=1;
    data=i2c_read();
    i2c_stop();
    return (data);
}

void set_reg(unsigned char address, unsigned char x)     // sets time
{
    i2c_start();
    i2c_write(0b11010000);
    i2c_write(address);
    i2c_write(x);
    i2c_stop();
    return;
}

void main()
{
    unsigned char date,mnt,yr,sec,min,hrs,i;
    i2c_init();
    set_reg(0x00,0x45);      // seconds
    set_reg(0x01,0x57);      // Miniutes
    set_reg(0x02,0x66);      // Hours along with 12hour and AM/PM selection bit.
    set_reg(0x04,0x01);      // Date
    set_reg(0x05,0x08);      // Month
    set_reg(0x06,0x14);      // Year

    while(1)
    {
        i2c_init();
        sec = get_reg(0);
        min = get_reg(1);
        hrs = get_reg(2)&0x1F;
        date= get_reg(4);
        mnt = get_reg(5);
        yr  = get_reg(6);

        SSPCON1bits.SSPEN = 0;
        InitLCD();
        sprintf(str,"date:%02x:%02x:%02x",date,mnt,yr);
        SendInstruction(0x80);
        for(i=0;i<13;i++)
        {
        SendData(str[i]);
        }
        
        sprintf(str,"time:%02x:%02x:%02x",hrs,min,sec);
        SendInstruction(0xC0);
        for(i=0;i<13;i++)
        {
            SendData(str[i]);
        }
        lcd_delay(10000);
    }
}



