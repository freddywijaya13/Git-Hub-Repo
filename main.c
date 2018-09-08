#include <string.h>
#include <xc.h>
#include "config.h"
#include "audio_playback.h"
#include "lcd.h"
#include "uart4.h"
#include "clock_config.h"

#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_2

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

void handler(unsigned char* data){
    LCD_WriteStringAtPos("byte received", 0, 0);
    LCD_WriteStringAtPos(data, 1, 0);
    UART_PutChar('k');
}

int main(){   
    LCD_Init(); 
    AUDIO_PLAY(); 
    while(1);
    return 0;
}