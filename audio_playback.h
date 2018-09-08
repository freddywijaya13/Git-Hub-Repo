#ifndef _AUDIO_PLAYBACK_H   
#define _AUDIO_PLAYBACK_H
void AUDIO_ConfigurePins();
void AUDIO_PLAY();
void AUDIO_Change_Freq(int value);
int AUDIO_get_freq();
void low_pass_filter();
int * AUDIO_get_outputs();
int * AUDIO_get_intputs();

#endif 