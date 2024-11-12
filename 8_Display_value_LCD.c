#include <p18f4550.h>
#include <stdio.h>

#define LCD_EN LATAbits.LATA1
#define LCD_RS LATAbits.LATA0
#define LCDPORT LATB

void lcd_delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 50; j++);
}

void SendInstruction(unsigned char command) {
    LCD_RS = 0;
    LCDPORT = command;
    LCD_EN = 1;
    lcd_delay(10);
    LCD_EN = 0;
    lcd_delay(10);
}

void SendData(unsigned char lcddata) {
    LCD_RS = 1;
    LCDPORT = lcddata;
    LCD_EN = 1;
    lcd_delay(10);
    LCD_EN = 0;
    lcd_delay(10);
}

void InitLCD(void) {
    ADCON1 = 0X0F;
    TRISB = 0x00;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;

    SendInstruction(0x38);
    SendInstruction(0x06);
    SendInstruction(0x0C);
    SendInstruction(0x01);
    SendInstruction(0x80);
}

void ADCInit(void) {
    TRISEbits.RE1 = 1;
    TRISEbits.RE2 = 1;
    ADCON1 = 0b00000111;
    ADCON2 = 0b10101110;
}

unsigned short Read_ADC(unsigned char Ch) {
    ADCON0 = 0b00000001 | (Ch << 2);
    GODONE = 1;
    while (GO_DONE == 1);
    return ADRES;
}

void DisplayResult(unsigned short ADCVal) {
    unsigned char i, text[16];
    unsigned short tempv;

    tempv = ADCVal;

    SendInstruction(0x80);
    for (i = 0; i < 10; i++) {
        if (tempv & 0x200) {
            SendData('1');
        } else {
            SendData('0');
        }
        tempv = tempv << 1;
    }

    ADCVal = (5500 / 1024) * ADCVal;
    sprintf(text, "ADC value=%4dmV", ADCVal);

    SendInstruction(0xC0);
    for (i = 0; i < 16; i++) {
        SendData(text[i]);
    }
}

void main() {
    unsigned short Ch_result;

    TRISB = 0x00;
    ADCInit();
    InitLCD();

    while (1) {
        Ch_result = Read_ADC(7);
        DisplayResult(Ch_result);
        lcd_delay(1000);
    }
}
