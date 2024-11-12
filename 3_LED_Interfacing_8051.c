#include <reg51.h>
#define display_port P0         // Data pins connected to port 0 on microcontroller
void delay(int);
void lcd_comm(void);
void lcd_data(void);
void lcd_init(void);
void lcd_clear(void);

unsigned char temp1, temp2, var, j;
unsigned char *ptr, disp1[] = "DVVP ", disp2[] = "LCD Interfacing";
sbit RS = P2^3;   // RS pin connected to P2.3
sbit EN = P2^5;   // E pin connected to P2.5
sbit RW = P2^4;   // RW pin connected to P2.4

int main()
{
    lcd_init();          // LCD initialization
    delay(100);
    lcd_clear();         // Clear display
    delay(100);

    while(1)
    {
        temp1 = 0x80;    // Starting address of first line
        lcd_comm();
        delay(50000);
        ptr = disp1;
        while(*ptr != '\0')   // Display first line
        {
            temp2 = *ptr;
            lcd_data();
            ptr++;
            delay(50000);
        }

        temp1 = 0xC0;    // Starting address of second line
        lcd_comm();
        delay(50000);
        ptr = disp2;
        while(*ptr != '\0')   // Display second line
        {
            temp2 = *ptr;
            lcd_data();
            ptr++;
            delay(50000);
        }

        temp1 = 0x01;   // Clear display command
        lcd_comm();
        delay(2000);
    }
}

void lcd_init()
{
    unsigned int cmd[] = {0x20, 0x28, 0x0E, 0x06, 0x80, 0x01};
    unsigned int i;
    for(i = 0; i < 6; i++)  // Loop for 6 commands only
    {
        temp1 = cmd[i];
        lcd_comm();
        delay(500);
    }
}

void lcd_comm()      // Send command to LCD
{
    var = temp1;
    temp1 = (temp1 & 0xF0) >> 4;  // Send higher nibble
    display_port = temp1;
    RS = 0;
    EN = 1;
    EN = 0;

    temp1 = var & 0x0F;  // Send lower nibble
    display_port = temp1;
    RS = 0;
    EN = 1;
    EN = 0;
    delay(60);
}

void lcd_data()      // Send data to LCD
{
    var = temp2;
    temp2 = (temp2 & 0xF0) >> 4;  // Send higher nibble
    display_port = temp2;
    RS = 1;
    EN = 1;
    EN = 0;

    temp2 = var & 0x0F;  // Send lower nibble
    display_port = temp2;
    RS = 1;
    EN = 1;
    EN = 0;
    delay(600);
}   

void lcd_clear(void)
{
    temp1 = 0x01;    // Clear display command
    lcd_comm();
    delay(500);					
}

void delay(int j)
{
    unsigned int i;
    for(i = 0; i < j; i++);  // Simple delay loop
}
