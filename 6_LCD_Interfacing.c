#include <p18f4550.h>
#define LCDPORT LATB          // Define LCDPORT as LATB
#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0

void lcd_delay(unsigned int time)
{
    unsigned int i, j;
    for(i = 0; i < time; i++)
    {
        for(j = 0; j < 100; j++);  // Simple delay loop
    }
}

void SendInstruction(unsigned char instruction)
{
    LCD_RS = 0;               // Instruction mode
    LCDPORT = instruction;    // Send command to LCDPORT
    LCD_EN = 1;               // Enable pulse
    lcd_delay(10);            // Short delay
    LCD_EN = 0;               // Disable pulse
    lcd_delay(10);            // Short delay
}

void SendData(unsigned char lcddata)
{
    LCD_RS = 1;               // Data mode
    LCDPORT = lcddata;        // Send data to LCDPORT
    LCD_EN = 1;               // Enable pulse
    lcd_delay(10);            // Short delay
    LCD_EN = 0;               // Disable pulse
    lcd_delay(10);            // Short delay
}

void InitLCD(void)
{
    ADCON1 = 0x0F;            // Configure all pins as digital
    TRISB = 0x00;             // Set PORTB as output for LCD data
    TRISAbits.RA0 = 0;        // Set RA0 as output for LCD_RS
    TRISAbits.RA1 = 0;        // Set RA1 as output for LCD_EN

    // Initialize LCD in 8-bit mode
    SendInstruction(0x38);    // Function set: 8-bit, 2 lines, 5x8 font
    SendInstruction(0x06);    // Entry mode: cursor moves right
    SendInstruction(0x0C);    // Display ON, Cursor OFF
    SendInstruction(0x01);    // Clear display
    SendInstruction(0x80);    // Set cursor to home position
}


//**********************************************************************

#include <p18f4550.h>
#define LCDPORT LATB
#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0

unsigned char *String1 = "Microembedded";
unsigned char *String2 = "PIC-18F Board";

void main(void)
{
    ADCON1 = 0x0F;
    TRISB = 0x00;
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;

    SendInstruction(0x38);  // 8-bit, 2-line display
    SendInstruction(0x06);  // Entry mode
    SendInstruction(0x0C);  // Display on, cursor off
    SendInstruction(0x01);  // Clear display
    SendInstruction(0x80);  // First line

    // Display String1 on the first line
    while(*String1)
    {
        SendData(*String1);
        String1++;
    }

    // Move to the second line and display String2
    SendInstruction(0xC0);
    while(*String2)
    {
        SendData(*String2);
        String2++;
    }

    while(1);  // Infinite loop to keep display active
}
