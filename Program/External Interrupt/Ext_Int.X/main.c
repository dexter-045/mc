#include <pic18f4550.h>

#define BUZZER_PIN LATAbits.LATA5

void interrupt extint_isr(void)
{
    unsigned int i;
    if(INT1F)
    {
        INT1F = 0;
        INT1IE = 0;
        BUZZER_PIN = ~BUZZER_PIN;
        for(i=0; i<10000; i++);      //small delay for debouncing
        INT1IE = 1;
    }
}

int main()
{
    ADCON1 = 0x0F;          //set pins as Digital
    TRISAbits.TRISA5 = 0;   //set buzzer pin RA5 as output
    TRISBbits.TRISB1 = 1;   //Interrupt pin as input
    BUZZER_PIN = 1;

    INT1IE  =   1;                      //Enable external interrupt INT1
    INTEDG1 =   0;                      //Interrupt on falling edge
    GIE     =   1;                      // Enable global interrupt

    while(1);
}
