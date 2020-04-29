/*
 * *****************************************************************************
 * SWITCH_TEST
 * *****************************************************************************
 * Program for an endurance test of a mechanical switch
 * The switch will be pushed 100'000 times or more.
 * The number of pushes will be counted and displayed.
 * *****************************************************************************
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
 */


#include <Arduino.h>
#include <Nextion.h>         // https://github.com/itead/ITEADLIB_Arduino_Nextion
#include <Debounce.h>        // https://github.com/chischte/debounce-library.git
#include <EEPROM_Counter.h>  // https://github.com/chischte/eeprom-counter-library.git
#include <Insomnia.h>        // https://github.com/chischte/insomnia-delay-library.git


enum counter {
  longTimeCounter, // name of the counter
  endOfEnum //end of the counter list
};
int numberOfValues = endOfEnum;

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P)
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560)
int eepromMinAddress = 0;
int eepromMaxAddress = 4095;

// CREATE THE EEPROM COUNTER:
EEPROM_Counter switchCounter;

// CREATE THE TIMEOUT TIMER:
Insomnia timeout(5000);

// CREATE A BLINK DELAY:
Insomnia blinkDelay;
//*****************************************************************************
// DECLARATION OF VARIABLES
//*****************************************************************************
// bool (true/false)
// byte (0-255)
// int   (-32,768 to 32,767)
// long  (-2,147,483,648 to 2,147,483,647)
// float (6-7 Digits)
//*****************************************************************************

/*
 * *****************************************************************************
 * Nextion.ino
 * configuration of the Nextion touch display
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
*/

//*****************************************************************************
// DECLARATION OF VARIABLES FOR NEXTION
//*****************************************************************************
bool resetStopwatchActive = false;
bool nextionPlayPauseButtonState;
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




// KNOBS AND POTENTIOMETERS:
const byte TEST_SWITCH_PIN = 2;
Debounce testSwitch(TEST_SWITCH_PIN);
const byte MOTOR_RELAY_PIN = 50;

// SENSORS:
// n.a.

// OTHER VARIABLES:
bool previousButtonState;
bool previousMachineState;
bool machineRunning = false;
bool buttonBlinkEnabled = false;




void send_to_nextion() {
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}


//*****************************************************************************
void updateDisplayCounter() {
  long newValue = switchCounter.getValue(longTimeCounter);
  Serial2.print("t0.txt=");
  Serial2.print("\"");
  Serial2.print(newValue);
  Serial2.print("\"");
  send_to_nextion();
}

void buttonBlink() {
  if (buttonBlinkEnabled) {
    if (blinkDelay.delayTimeUp(500)) {
      Serial2.print("click bt0,1"); // click button
      send_to_nextion();
    }
  }
}

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

  // TOGGLE APPEARANCE OF PLAY/PAUSE BUTTON:
  Serial2.print("click bt0,1");        // click button
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
// END OF TOUCH EVENT FUNCTIONS
//*****************************************************************************



//*****************************************************************************
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup() {
  switchCounter.setup(eepromMinAddress, eepromMaxAddress, numberOfValues);
  Serial.begin(9600); // start serial connection
  pinMode(TEST_SWITCH_PIN, INPUT_PULLUP);
  pinMode(MOTOR_RELAY_PIN, OUTPUT);
  digitalWrite(MOTOR_RELAY_PIN, HIGH);
  nextionSetup();
  timeout.setActive(false); // timeout will be set active later
  updateDisplayCounter();
  Serial.println("EXIT SETUP");
}
//*****************************************************************************
//********************#*********#####***#####***######*************************
//********************#********#*****#*#*****#**#*****#************************
//********************#********#*****#*#*****#**######*************************
//********************#********#*****#*#*****#**#******************************
//********************#######***#####***#####***#******************************
//*****************************************************************************

void loop() {

  // GET INFOS FROM TOUCH DISPLAY:
  nextionLoop();

  // DETECT IF MACHINE HAS BEEN SWITCHED ON:
  if (machineRunning) {
    if (machineRunning == !previousMachineState) {
      timeout.setActive(machineRunning);
      Serial.println("MACHINE SWITCHED ON");
      previousMachineState = machineRunning;
    }
  }

  // DETECT IF MACHINE HAS BEEN SWITCHED OFF:
  if (!machineRunning) {
    if (machineRunning == !previousMachineState) {
      timeout.setActive(machineRunning);
      Serial.println("MACHINE SWITCHED OFF");
      previousMachineState = machineRunning;
    }
  }

  // SWITCH MOTOR ON AND OFF:
  digitalWrite(MOTOR_RELAY_PIN, !machineRunning);

  // GET SIGNAL FROM TEST SWITCH AND COUNT IT:
  bool debouncedButtonState = testSwitch.requestButtonState();
  if (previousButtonState != debouncedButtonState) {
    if (debouncedButtonState == LOW) {
      switchCounter.countOneUp(longTimeCounter);
      updateDisplayCounter();
      timeout.resetTime();
      buttonBlinkEnabled = false;
    }
    previousButtonState = debouncedButtonState;
  }

  // SWITCH OFF IF TIME-OUT IS REACHED
  if (timeout.active()) {
    if (timeout.timedOut()) {
      machineRunning = false;
      buttonBlinkEnabled = true;
      Serial.println("TIMEOUT-REACHED");
    }
  }
  // LET THE DISPLAY BLINK IF THE TEST SWITCH FAILED:
  buttonBlink(); // button blinks only if blink has been enabled
}
