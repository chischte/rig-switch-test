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
bool reset_stopwatch_active = false;

//***************************************************************************
bool nex_prev_stepMode = true;
int currentPage = 0;
unsigned long counter_reset_stopwatch;
//**************************************************************************************
//DECLARATION OF OBJECTS TO BE READ FROM NEXTION
//**************************************************************************************

//PAGE 0:
NexDSButton nex_switch_play_pause = NexDSButton(0, 2, "bt0");

//**************************************************************************************
//END OF OBJECT DECLARATION
//**************************************************************************************

char buffer[100] = { 0 }; // This is needed only if you are going to receive a text from the display. You can remove it otherwise.

//**************************************************************************************
//TOUCH EVENT LIST //DECLARATION OF TOUCH EVENTS TO BE MONITORED
//**************************************************************************************
NexTouch *nex_listen_list[] = { &nex_switch_play_pause,

NULL //String terminated
        };
//**************************************************************************************
//END OF TOUCH EVENT LIST
//**************************************************************************************

void send_to_nextion() {
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
}

//**************************************************************************************
void nextionSetup()
//**************************************************************************************
{
    Serial.println("START OF NEXTION SETUP");
    Serial2.begin(9600);  // Start serial comunication at baud=9600

    //**************************************************************************************
    //REGISTER THE EVENT CALLBACK FUNCTIONS
    //**************************************************************************************
    //*****PUSH::
    // n.a.

    //*****PUSH+POP:
    nex_switch_play_pause.attachPush(nex_switch_play_pausePushCallback);
    nex_switch_play_pause.attachPop(nex_switch_play_pausePopCallback);

    //**************************************************************************************
    //END OF REGISTER
    //**************************************************************************************
    delay(2000);
    send_to_nextion();
    Serial.println("END OF NEXTION SETUP");

}  //END OF NEXTION SETUP

//**************************************************************************************
void nextionLoop()
//**************************************************************************************
{ //START NEXTION LOOP
//**************************************************************************************
    nexLoop(nex_listen_list); //check for any touch event
    //**************************************************************************************
    if (currentPage == 0) {
        //*******************
        //PAGE 1:
        //*******************

        //UPDATE SWITCHSTATE "PLAY"/"PAUSE"
        if (nex_state_machineRunning != machineRunning) {
            Serial2.print("click bt0,1");    //CLICK BUTTON
            send_to_nextion();
            nex_state_machineRunning = !nex_state_machineRunning;
        }
        //RESET COUNTER:
        if (reset_stopwatch_active == true) {
            if (millis() - counter_reset_stopwatch > 3000) {
                switchCounter.set(longTimeCounter, 0);
                updateDisplayCounter();
            }
        }
    }
}    //END OF NEXTION LOOP

//**************************************************************************************
//TOUCH EVENT FUNCTIONS //PushCallback = Press event //PopCallback = Release event
//**************************************************************************************

void nex_switch_play_pausePushCallback(void *ptr) {

    machineRunning = !machineRunning;
    if (machineRunning == true) {
        clearanceNextStep = true;
    }
    if (machineRunning == false) {

    }
    nex_state_machineRunning = !nex_state_machineRunning;

    counter_reset_stopwatch = millis();
    reset_stopwatch_active = true;

}

void nex_switch_play_pausePopCallback(void *ptr) {
    reset_stopwatch_active = false;
}

//**************************************************************************************
//END OF TOUCH EVENT FUNCTIONS
//**************************************************************************************
