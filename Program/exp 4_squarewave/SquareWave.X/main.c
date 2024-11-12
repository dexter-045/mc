/*CALCULATIONS
 * required time = 100ms
 * TMR value = 0xFFFF - (required time) / (4 * Tosc * Prescaler)
 *           = 0xFFFF - (0.1 * 48000000) / (4 * 256)
 *           = 0xFFFF - 0x124F
 * TMR       = 0xEDB0
 * TMRH = 0xED
 * TMRL = 0xB0
 */

#include<p18f4550.h>

volatile bit timer_set = 0;

void timerInit(void)
{
     // Timer0 configuration
    T0CON = 0b00000111;                     //Timer0 16-bit; prescaler 1:256
    TMR0H = 0xED;
    TMR0L = 0xB0;
}

void Interrupt_Init(void)
{
    RCONbits.IPEN = 1;
    INTCON = 0b11100000;                    //Enable global and Timer0 interrupts; Clear Timer0 interrupt flag
    INTCON2bits.TMR0IP = 0;
}

void interrupt low_priority timerinterrupt(void)
{
    if(TMR0IF == 1)                         //If timer0 interrupt flag is set.....
    {
      TMR0ON = 0;                           // Stop the timer
      TMR0IF = 0;                           //Clear the interrupt flag
      TMR0H = 0xED;                         //Reload Timer0
      TMR0L = 0xB0;
      LATB =~LATB;                          //Toggle PORTB
      TMR0ON = 1;                           // Start the timer
    }
}


void main(void)
{
   TRISB = 0x00;
   LATB = 0xFF;

   Interrupt_Init();
   timerInit();
   TMR0ON = 1;                              // Start the timer

   while(1);                                //Loop forever; do nothing
}


