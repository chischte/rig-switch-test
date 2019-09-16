/*
 * *****************************************************************************
 * Nextion.ino
 * configuration of the Nextion touch display
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
 * An Excel-sheet to generate Nextion events can be found here:
 * https://github.com/chischte/push-type-rig/PUSHTYPE_NEXTION/
 * *****************************************************************************
 * Include the Nextion ITEADLIB library https://github.com/itead/ITEADLIB_Arduino_Nextion
 * Make sure you edit the NexConfig.h file on the library folder to set the correct
 * serial port for the display.
 * By default it's set to Serial1, which most Arduino boards don't have.
 * Change "#define nexSerial Serial1" to "#define nexSerial Serial" if you are
 * using Arduino uno, nano, etc.
 * *****************************************************************************
 * VARIOUS COMMANDS:
 * *****************************************************************************
 * Serial2.print("click bt1,1");// (button 1, press event)
 * send_to_nextion();
 * A switch (Dual State Button) will be toggled with this command,
 * a Button will be set permanently pressed
 * Serial2.print("vis t0,0");// HIDE OBJECT
 * send_to_nextion();
 * Serial2.print("t0.txt=");// WRITE TEXT:
 * Serial2.print("\"");
 * *****************************************************************************
 */

//*****************************************************************************
// DECLARATION OF VARIABLES
//*****************************************************************************
bool resetStopwatchActive = false;
bool counterReseted = false;
int currentPage = 0;
unsigned long counterResetStopwatch;

//*****************************************************************************
// DECLARATION OF OBJECTS TO BE READ FROM NEXTION
//*****************************************************************************
// PAGE 0:
NexDSButton nex_switch_play_pause = NexDSButton(0, 2, "bt0");

//*****************************************************************************
// END OF OBJECT DECLARATION
//*****************************************************************************

char buffer[100] = { 0 }; // This is needed only if you are going to receive a text from the display.

//*****************************************************************************
// TOUCH EVENT LIST //DECLARATION OF TOUCH EVENTS TO BE MONITORED
//*****************************************************************************
NexTouch *nex_listen_list[] = { &nex_switch_play_pause,

NULL //String terminated
        };
//*****************************************************************************
// END OF TOUCH EVENT LIST
//*****************************************************************************

void send_to_nextion() {
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
}

//*****************************************************************************
void nextionSetup()
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
    nex_switch_play_pause.attachPush(nex_switch_play_pausePushCallback);
    nex_switch_play_pause.attachPop(nex_switch_play_pausePopCallback);

    //*****************************************************************************
    // END OF REGISTER
    //*****************************************************************************
    delay(2000);
    send_to_nextion();
    Serial.println("END OF NEXTION SETUP");

}  // END OF NEXTION SETUP

//*****************************************************************************
void nextionLoop()
//*****************************************************************************
{
    nexLoop(nex_listen_list); //check for any touch event

    if (currentPage == 0) {
        //********************************
        // PAGE 0:
        //********************************
        // RESET COUNTER:
        if (resetStopwatchActive == true) {
            if (millis() - counterResetStopwatch > 3000) {
                switchCounter.set(longTimeCounter, 0);
                updateDisplayCounter();
                counterReseted = true;
            }
        }
    }
}    //END OF NEXTION LOOP

//*****************************************************************************
// TOUCH EVENT FUNCTIONS
//*****************************************************************************

void nex_switch_play_pausePushCallback(void *ptr) {
    counterResetStopwatch = millis();
    resetStopwatchActive = true;
}

void nex_switch_play_pausePopCallback(void *ptr) {

    if (counterReseted == false) {
        machineRunning = !machineRunning;
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
// END OF TOUCH EVENT FUNCTIONS
//*****************************************************************************
