/*
 * *****************************************************************************
   nextion.ino
   configuration of the Nextion touch display
   Michael Wettstein
   September 2019, Zürich
 * *****************************************************************************
   an xls-sheet to generate Nextion events can be found here:
   https://github.com/chischte/push-type-rig/PUSHTYPE_NEXTION/
 * *****************************************************************************
*/

// Include the nextion library (the official one) https://github.com/itead/ITEADLIB_Arduino_Nextion
// Make sure you edit the NexConfig.h file on the library folder to set the correct serial port for the display.
// By default it's set to Serial1, which most arduino boards don't have.
// Change "#define nexSerial Serial1" to "#define nexSerial Serial" if you are using arduino uno, nano, etc.
//**************************************************************************************
//VARIOUS COMMANDS:
//**************************************************************************************
// Serial2.print("click bt1,1");//CLICK BUTTON
// send_to_nextion();
// A switch (Dual State Button)will be toggled with this command, a Button will be set permanently pressed)
// Serial2.print("vis t0,0");//HIDE OBJECT
// send_to_nextion();
// Serial2.print("t0.txt=");//WRITE TEXT:
// Serial2.print("\"");
// Serial2.print("SCHNEIDEN");
// Serial2.print("\"");
// send_to_nextion();
//**************************************************************************************
//DECLARATION OF VARIABLES
//**************************************************************************************


//***************************************************************************
//NEXTION SWITCH STATES LIST
//Every nextion switch button (dualstate) needs a switchstate variable to control switchtoggle
//Nextion buttons(momentary) need a variable too, to prevent screen flickering

bool nex_state_machineRunning;
bool reset_stopwatch_active;

//***************************************************************************
bool nex_prev_stepMode = true;

unsigned int stopped_button_pushtime;
unsigned long counter_reset_stopwatch;
unsigned long nex_prev_longtimeCounter;
unsigned long nex_prev_shorttimeCounter;
//**************************************************************************************
//DECLARATION OF OBJECTS TO BE READ FROM NEXTION
//**************************************************************************************

//PAGE 0:
NexPage nex_page0 = NexPage(0, 0, "page1");
NexDSButton nex_switch_play_pause = NexDSButton(0, 2, "bt0");

//**************************************************************************************
//END OF OBJECT DECLARATION
//**************************************************************************************

char buffer[100] =
{ 0 }; // This is needed only if you are going to receive a text from the display. You can remove it otherwise.

//**************************************************************************************
//TOUCH EVENT LIST //DECLARATION OF TOUCH EVENTS TO BE MONITORED
//**************************************************************************************
NexTouch *nex_listen_list[] =
{ &nex_switch_play_pause,

  NULL //String terminated
};
//**************************************************************************************
//END OF TOUCH EVENT LIST
//**************************************************************************************

void send_to_nextion()
{
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

//**************************************************************************************
void nextionSetup()
//**************************************************************************************
{
  Serial2.begin(9600);  // Start serial comunication at baud=9600

  //**************************************************************************************
  //REGISTER THE EVENT CALLBACK FUNCTIONS
  //**************************************************************************************
  nex_switch_play_pause.attachPush(nex_switch_play_pausePushCallback);

  //**************************************************************************************
  //END OF REGISTER
  //**************************************************************************************
  delay(2000);
  send_to_nextion();

}  //END OF NEXTION SETUP

//**************************************************************************************
void nextionLoop()
//**************************************************************************************
{ //START NEXTION LOOP

  if (nex_prev_shorttimeCounter != shorttimeCounter)
  {
    Serial2.print("t0.txt=");
    Serial2.print("\"");
    Serial2.print(shorttimeCounter);
    Serial2.print("\"");
    send_to_nextion();
    nex_prev_shorttimeCounter = shorttimeCounter;
  }
  if (reset_stopwatch_active == true)
  {
    if (millis() - counter_reset_stopwatch > 5000)
    {
      shorttimeCounter = 0;
      longtimeCounter = 0;
    }
  }

}    //END OF NEXTION LOOP

//**************************************************************************************
//TOUCH EVENT FUNCTIONS //PushCallback = Press event //PopCallback = Release event
//**************************************************************************************
void nex_switch_play_pausePushCallback(void *ptr)
{
  machineRunning = !machineRunning;
  if (machineRunning == true)
  {
    clearanceNextStep = true;
  }
  if (machineRunning == false)
  {

  }
  nex_state_machineRunning = !nex_state_machineRunning;
}

void nex_but_reset_shorttimeCounterPushCallback(void *ptr)
{
  shorttimeCounter = 0;
  //RESET LONGTIME COUNTER IF RESET BUTTON IS PRESSED LONG ENOUGH:
  counter_reset_stopwatch = millis();
  reset_stopwatch_active = true;

}
void nex_but_reset_shorttimeCounterPopCallback(void *ptr)
{
  reset_stopwatch_active = false;
}

//**************************************************************************************
//END OF TOUCH EVENT FUNCTIONS
//**************************************************************************************
