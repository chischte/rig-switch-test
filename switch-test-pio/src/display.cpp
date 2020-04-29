#include "display.h"
#include <Arduino.h>
#include <Nextion.h>  

// Constructor (Former Nextion Setup)
Display::Display(){

//*****************************************************************************
// DECLARATION OF OBJECTS TO BE READ FROM NEXTION
//*****************************************************************************
// PAGE 0:
NexDSButton nex_switch_play_pause = NexDSButton(0, 2, "bt0");

//*****************************************************************************
// END OF OBJECT DECLARATION
//*****************************************************************************






}

void Display::send_to_nextion() {
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void Display::updateDisplayCounter() {
 long newValue=678;
  //long newValue = switchCounter.getValue(longTimeCounter);
  Serial2.print("t0.txt=");
  Serial2.print("\"");
  Serial2.print(newValue);
  Serial2.print("\"");
  send_to_nextion();
}

void Display::nex_switch_play_pausePushCallback(void *ptr) {
  counterResetStopwatch = millis();
  resetStopwatchActive = true;
}
