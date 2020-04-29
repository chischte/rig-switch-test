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

Display::nextionSetup();

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

void Display::nex_switch_play_pausePopCallback(void *ptr) {

  if (counterReseted == false) {
   // machineRunning = !machineRunning;
  } else {
    //counter has been reseted
    //change of machine state did not happen,
    //therefore switch the button layout back:
    Serial2.print("click bt0,1");        // click button
    send_to_nextion();
    counterReseted = false; // counter reset steps completed
  }
  resetStopwatchActive = false;
}

//*****************************************************************************
void Display::nextionSetup()
//*****************************************************************************
{
  Serial.println("START OF NEXTION SETUP");
  Serial2.begin(9600);  // Start serial comunication at baud=9600

  //*****************************************************************************
  // REGISTER THE EVENT CALLBACK FUNCTIONS
  //*****************************************************************************
  //*****PUSH::
  // n.a.

  //*****PUSH+POP:
  //nex_switch_play_pause.attachPush(nex_switch_play_pausePushCallback);
  //nex_switch_play_pause.attachPop(nex_switch_play_pausePopCallback);

  //*****************************************************************************
  // END OF REGISTER
  //*****************************************************************************
  delay(2000);
  send_to_nextion();

  // TOGGLE APPEARANCE OF PLAY/PAUSE BUTTON:
  Serial2.print("click bt0,1");        // click button
  send_to_nextion();

  Serial.println("END OF NEXTION SETUP");

}  // END OF NEXTION SETUP

//*****************************************************************************
void Display::nextionLoop()
//*****************************************************************************
{
  //nexLoop(nex_listen_list); //check for any touch event

  if (currentPage == 0) {
    //********************************
    // PAGE 0:
    //********************************
    // RESET COUNTER:
    if (resetStopwatchActive == true) {
      if (millis() - counterResetStopwatch > 3000) {
    //    switchCounter.set(longTimeCounter, 0);
        updateDisplayCounter();
        counterReseted = true;
      }
    }
  }
}    //END OF NEXTION LOOP
