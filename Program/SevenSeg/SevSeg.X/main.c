/*
 * File:   main.c
 * Author: MicroEmbedded
 *
 * Created on October 15, 2016, 4:17 PM
 */


#include <pic18f4550.h>
#define ONE     0b0000110
#define TWO     0b1011011
#define THREE   0b1001111
#define FOUR    0b1100110
#define FIVE    0b1101101
#define SIX     0b1111101
#define SEVEN   0b0000111
#define EIGHT   0b1111111
#define NINE    0b1101111
#define ZERO    0b0111111
#define SEG1    LATAbits.LATA0
#define SEG2    LATAbits.LATA1

const unsigned char segData[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX,SEVEN, EIGHT, NINE};

void delay(unsigned int time)
{
    int i;
    while(time--)
        for(i=0;i<500;i++);
}

int main(void) 
{
    unsigned char count = 0;
    unsigned char digit1, digit2;
    unsigned int i;
    TRISB = 0x00;
    LATA =  0x00;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    while(1)
    {
        for(count=0;count<100;count++)
        {
            digit1 = count / 10;
            digit2 = count % 10;
            for(i=0;i<100;i++)
            {
                SEG1 = 1;
                SEG2 = 0;
                LATB = ~segData[digit1];
                delay(2);
                SEG1 = 0;
                SEG2 = 1;
                LATB = ~segData[digit2];
                delay(2);
            }
        }
    }
    return 0;
}
