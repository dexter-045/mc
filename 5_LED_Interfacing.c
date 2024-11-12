#include <p18f4550.h>

#define TOGGLE_DELAY 200

void delay(unsigned int time)
{
    unsigned int i, j;
    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 5000; j++)
        {
            // Simple delay
        }
    }
}

void main(void)
{
    TRISB = 0x00;      // Set Port B as output
    LATB = 0xFF;       // Initially turn on all LEDs

    while (1)
    {
        LATB = ~LATB;  // Toggle all LEDs
        delay(TOGGLE_DELAY);  // Delay for visible toggling effect
    }
}
