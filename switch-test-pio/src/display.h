#ifndef __DISPLAY_H_INCLUDED__
#define __DISPLAY_H_INCLUDED__
#include <Nextion.h>  

class Display{

private:



char buffer[100] = { 0 }; // This is needed only if you are going to receive a text from the display.


void send_to_nextion();
void nex_switch_play_pausePushCallback(void *ptr);
void nex_switch_play_pausePopCallback(void *ptr);

void createPushFunction();


public:
NexDSButton nex_switch_play_pause = NexDSButton(0, 2, "bt0");
bool resetStopwatchActive = false;
bool nextionPlayPauseButtonState;
bool counterReseted = false;
int currentPage = 0;
unsigned long counterResetStopwatch;


void nextionSetup();
void updateDisplayCounter();
void nextionLoop();

Display();


};

#endif /* DISPLAY_H */