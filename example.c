#include <stdio.h>
#include "lcd.h"
void example(){
    LCD_Init();
    LCD_WriteStringAtPos("Hello",0,0);
}