#include<pic18f4550.h>
#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0
#define LCDPORT LATB
#define SLP 0                   //for SLEEP mode
#define IDLE 1                  //for IDLE mode


void delay(unsigned int time)
{
 unsigned int i , j ;

    for(i = 0; i < time; i++)
    {
            for(j=0;j<200;j++);
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
    SendInstruction(0x80);      //set address to 1st line

}


void Enable_Interrupt(void)     //Enable external interrupt and RB1 as input
{
    GIE = 1;
    INT1IE = 1;
    TRISBbits.TRISB1 = 1;
}

void Disable_Interrupt(void)    //Disable external interrupt and RB1 as output for LCD
{
    GIE = 0;
    INT1IE = 0;
    TRISBbits.TRISB1 = 0;
}


void interrupt isr(void)
{
    if(INT1IF)          //if ext interrupt flag is set
    {
        INT1IF = 0;     //clear flag; wake up from sleep mode
    }
}

void power_down(unsigned int mode)
{
    unsigned char *string = "POWER DOWN mode";
    int i;
    SendInstruction(0x01);
    SendInstruction(0x80);
    for(i=0;i<15;i++)
         SendData(string[i]);
    IDLEN = mode;          //IDLEN = 0, SLEEP mode on SLEEP instruction
                        //IDLEN = 1, IDLE mode on SLEEP instruction
    Enable_Interrupt();
    asm ("sleep");      //Enter SLEEP or IDLE mode depending on IDLEN bit
}

void main()
{
    unsigned char string[12] = "RUN mode....";
    int i,j;
    InitLCD();

    while(1)
    {
        Disable_Interrupt();
        SendInstruction(0x01);
        for(j=0;j<200;j++)
        {
            SendInstruction(0x80);
            for(i=0;i<12;i++)
                SendData(string[i]);
            delay(20);
        }
        power_down(SLP);
    }

}