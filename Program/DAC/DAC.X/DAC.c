/*connect CN3 of DAC board to CN13 of PIC18F board*/

#include <p18f4550.h>

void delay(unsigned int time)
{
    unsigned int i,j;
    for(i=0;i<time;i++)
        for(j=0;j<100;j++);
}

void main(void)
{
    unsigned int i,j;

   TRISB = 0x00;
   LATB = 0xFF;

while(1)
{
   for(i=0;i<255;i++)
   {
       LATB = i;
       delay(5);
   }
   for(i=0;i<255;i++)
   {
       LATB = (255-i);
       delay(5);
   }
}
}



