#include <p18f4550.h>
#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0
#define LCDPORT LATB

const unsigned char KeyLookupTbl[]={'1','4','7','.',
   				    '2','5','8','0',
				    '3','6','9','#',
				    'A','B','C','D'};

void lcd_delay(unsigned int time)
{
 unsigned int i , j ;

    for(i = 0; i < time; i++)
    {
            for(j=0;j<50;j++);
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
    SendInstruction(0x06);	// entry mode
    SendInstruction(0x0C);	//Display ON cursor OFF
    SendInstruction(0x01);      //Clear display
    SendInstruction(0x80);      //set address to 0
}



/*Reads a Single key*/
//Returns the ascii value of the key
unsigned char ReadKey(void)
{
 unsigned char row,val, i, j, key=0;

 while(1)                                   //Loop till a key is pressed
 {
   LATD = 0xFF;
   for(i=0x01;i<0x10;i=i<<1)
   {
       LATD = ~i;                           //Make output pin of PORTD low, one column at a time
       lcd_delay(2);
       row = PORTD>>4;                      //Scan rows
       for(j=0x01;j<0x10;j=j<<1)
       {
           if((row & j) == 0)                 //Check which row scanned is low
           {
               val = KeyLookupTbl[key];     //If a key is pressed, find and return the corresponding character
               return val;
           }
           else
               key++;                       //If key not pressed, increament key counter value
       }
   }

 }
}

void main()
{
    unsigned char Key, str[16];
    unsigned char *string1 = "Key Pressed = ";
    
    TRISD = 0xF0;                           //rows as inputs and columns as output
    LATD = 0xFF;

    InitLCD();
    SendInstruction(0x80);                  //set 1st line
    while(*string1)
        SendData(*string1++);


     while(1)                               //Forever loop
    {
        Key = ReadKey();                    //Check the key pressed
        SendInstruction(0xC0);              //set 2nd line
        SendData(Key);
        lcd_delay(100);
     }
}






