#ifndef __DISPLAY_H_INCLUDED__
#define __DISPLAY_H_INCLUDED__


class Display{

private:
void send_to_nextion();
void nex_switch_play_pausePushCallback(void *ptr);



public:

bool resetStopwatchActive = false;
bool nextionPlayPauseButtonState;
bool counterReseted = false;
int currentPage = 0;
unsigned long counterResetStopwatch;


void updateDisplayCounter();



Display();


};

#endif /* DISPLAY_H */