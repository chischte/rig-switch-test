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
int eepromSize = 4096;

// CREATE THE EEPROM COUNTER:
EEPROM_Counter switchCounter(eepromSize, numberOfValues);

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
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup() {
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
