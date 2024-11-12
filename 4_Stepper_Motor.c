#include <reg52.h>
#include <stdio.h>

void delay(int);

void main()
{
  while (1)
  {
    P1 = 0x03;        // Step 1: Energize first coil
    delay(1000);
    P1 = 0x06;        // Step 2: Energize second coil
    delay(1000);
    P1 = 0x0C;        // Step 3: Energize third coil
    delay(1000);
    P1 = 0x09;        // Step 4: Energize fourth coil
    delay(1000);
  }
}

void delay(int k)
{
  int i, j;
  for (i = 0; i < k; i++)
  {
    for (j = 0; j < 100; j++)
    {
      // Simple delay loop
    }
  }
}
