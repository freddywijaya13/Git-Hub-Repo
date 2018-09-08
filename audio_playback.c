#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "config.h"
#include "clock_config.h"
#include "audio_playback.h"
#include "eeprom.h"
#include "lcd.h"

#define NUM_OF_SAMPLE 1319706

int PLAYBACK_FREQ = 80000;
unsigned int inputs[3]={0,0,0},outputs[3]={0,0,0};

long int audio_sample=0;
unsigned int count = 0;
char sample[1024];
unsigned int sample_count = 0;

void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer3ISR(void) 
{   
    count++;
    if(count==10){
        count = 0;
        if(sample_count == 1024){
            EEPROM_fast_read_data(sample,audio_sample,1024);
            OC1RS=(int)(sample[0]+250);
            sample_count = 1;  
            audio_sample+=1024;
        }else {
            OC1RS=(int)(sample[sample_count]+250);
            sample_count++;
        }     
    }
    audio_sample%= NUM_OF_SAMPLE;
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}

void AUDIO_PLAY()
{   
    AUDIO_ConfigurePins();
    PR3 = (int)((float)((float)PB_FRQ/PLAYBACK_FREQ) + 0.5); 
    TMR3 = 0; 
    T3CONbits.TCKPS = 0;     //1:1 prescale value
    T3CONbits.TGATE = 0;     //not gated input (the default)
    T3CONbits.TCS = 0;       //PCBLK input (the default)
    T3CONbits.ON = 1;        //turn on Timer3
 
    OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
    OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
    OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC1CONbits.ON = 1;       // Start the OC1 module  

    IPC3bits.T3IP = 7;      // interrupt priority
    IPC3bits.T3IS = 3;      // interrupt subpriority
    IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
    EEPROM_init(20000000);
    EEPROM_fast_read_data(sample,audio_sample,1024);
    OC1RS=sample[0]+250;
    sample_count = 1;
    macro_enable_interrupts();  // enable interrupts at CPU
}

void AUDIO_ConfigurePins()
{
    // Configure AUDIO output as digital output.
    tris_A_OUT = 0;    
    rp_A_OUT = 0x0C; // 1100 = OC1
    // disable analog (set pins as digital)
    ansel_A_OUT = 0;
}

void AUDIO_Close()
{
        T3CONbits.ON = 0;       // turn off Timer3
        OC1CONbits.ON = 0;      // Turn off OC1
        
}

void AUDIO_Change_Freq(int value){
    macro_disable_interrupts;
    PLAYBACK_FREQ +=value;
    PR3 = (int)((float)((float)PB_FRQ/(PLAYBACK_FREQ)) + 0.5);   
    macro_enable_interrupts();
}

//int AUDIO_get_freq(){
//    return Wav_file[audio_sample];
//}

int * AUDIO_get_outputs(){
    return outputs;
}

int * AUDIO_get_inputs(){
    return inputs;
}

void low_pass_filter(){
    inputs[2]=inputs[1];
    inputs[1]=inputs[0];
    //inputs[0]=Wav_file[audio_sample]/8;
    outputs[2]=outputs[1];
    outputs[1]=outputs[0];
    outputs[0] = (int)(inputs[0]+inputs[1])/2;
    //outputs[0]=(int)((float)(0.22922*(float)outputs[1]-0.56557*(float)outputs[2]+1.7758*(float)inputs[0]+0.54375*(float)inputs[1]+1.7758*(float)inputs[2]));
    //audio_sample++;
}