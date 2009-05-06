//

#include <unistd.h>
#include <stdio.h>
#include <string>
#include "textlcd.h"

using namespace std;

int main(int argc, char **argv)
   {
   CTextLcd lcd;

   printf("Clearing LCD\n");
   lcd.Clear();
   sleep(1);

   printf("backlight on\n");
   lcd.SetProperty(TL_PROP_BACKLIGHT, true);
   sleep(1);

   printf("backlight off\n");
   lcd.SetProperty(TL_PROP_BACKLIGHT, false);
   sleep(1);

   printf("backlight on\n");
   lcd.SetProperty(TL_PROP_BACKLIGHT, true);
   sleep(1);

   printf("Printing 'Hello World' on line 0\n");
   lcd.SetLine(0, "Hello World");
   sleep(1);

   printf("Printing 'What up Moon' on line 1\n");
   lcd.SetLine(1, "What up Moon");
   sleep(1);

   printf("Printing 'Goodbye World' on line 0 (clearing it first)\n");
   lcd.SetLine(0, "Goodbye World");
   sleep(1);

   printf("Printing 'Peace Out Moon' on line 1 (clearing it first)\n");
   lcd.SetLine(1, "Peace Out Moon");
   sleep(1);

   printf("Printing 'GOODBYE' on line 0 (NOT clearing it first)\n");
   lcd.SetLine(0, "GOODBYE", false);
   sleep(1);

   printf("Printing 'PEACE OUT Moon' on line 1 (NOT clearing it first)\n");
   lcd.SetLine(1, "PEACE OUT Moon", false);
   sleep(1);

   printf("Clearing LCD\n");
   lcd.Clear();
   sleep(1);

   printf("Printing 20 digits on line 0 (NOT clearing it first)\n");
   lcd.SetLine(0, "01234567890123456789", false);
   sleep(1);

   printf("Printing 20 digits on line 1 (NOT clearing it first)\n");
   lcd.SetLine(1, "01234567890123456789", false);
   sleep(1);

   printf("Clearing Line 0\n");
   lcd.ClearLine(0);
   sleep(1);

   printf("Clearing Line 1\n");
   lcd.ClearLine(1);
   sleep(1);

   printf("Printing Characters...\n");
   lcd.SetCharacter(0, 0, 'A');
   lcd.SetCharacter(0, 2, 'B');
   lcd.SetCharacter(0, 4, 'C');
   lcd.SetCharacter(0, 6, 'D');
   lcd.SetCharacter(0, 8, 'E');
   lcd.SetCharacter(0, 10, 'F');
   lcd.SetCharacter(0, 12, 'G');
   lcd.SetCharacter(0, 14, 'H');
   lcd.SetCharacter(0, 16, 'I');
   lcd.SetCharacter(1, 1, 'J');
   lcd.SetCharacter(1, 3, 'K');
   lcd.SetCharacter(1, 5, 'L');
   lcd.SetCharacter(1, 7, 'M');
   lcd.SetCharacter(1, 9, 'N');
   lcd.SetCharacter(1, 11, 'O');
   lcd.SetCharacter(1, 13, 'P');
   lcd.SetCharacter(1, 15, 'Q');
   lcd.SetCharacter(1, 17, 'R');
   sleep(4);

   printf("Clearing LCD and quitting\n");
   lcd.Clear();
   }

