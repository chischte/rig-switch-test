/*
 * *****************************************************************************
 SWITCH_TEST
 * *****************************************************************************
 Program for a mechanical switch endurance test
 * *****************************************************************************
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 */
/*
 TODO:
 Write EEPROM library
 Implement debounce algorithm
 */

#include <Nextion.h>  // https://github.com/itead/ITEADLIB_Arduino_Nextion
#include <Debounce.h> // ...add github link
#include <EEPROM_Counter.h>

enum counter {
    longTimeCounter,    // example value name
    endOfEnum
};
int numberOfValues = endOfEnum;

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P)
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560)
int eepromSize = 4096;

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Counter switchCounter(eepromSize, numberOfValues);

//*****************************************************************************
//DECLARATION OF VARIABLES / DATA TYPES
//*****************************************************************************
//bool (true/false)
//byte (0-255)
//int   (-32,768 to 32,767) / unsigned int: 0 to 65,535
//long  (-2,147,483,648 to 2,147,483,647)
//float (6-7 Digits)
//*****************************************************************************

//KNOBS AND POTENTIOMETERS:
const byte TEST_SWITCH_PIN = 2;
const byte MOTOR_RELAY_PIN = 50;
//SENSORS:

//OTHER VARIABLES:
bool debouncedButtonState;
bool currentButtonState;
bool previousButtonState;
bool machineRunning = false;
bool clearancePlayPauseToggle = true;
bool clearanceNextStep = false;

unsigned long runtime;
unsigned long runtimeStopwatch;
unsigned long prev_time;

Debounce testSwitch(TEST_SWITCH_PIN);

//*****************************************************************************
void updateDisplayCounter() {
    long newValue = switchCounter.getValue(longTimeCounter);
    Serial2.print("t0.txt=");
    Serial2.print("\"");
    Serial2.print(newValue);
    Serial2.print("\"");
    send_to_nextion();
}
//*****************************************************************************
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup() {
    Serial.begin(9600); //start serial connection
    pinMode(TEST_SWITCH_PIN, INPUT_PULLUP);
    pinMode(MOTOR_RELAY_PIN, OUTPUT);
    nextionSetup();
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

    //GET INFOS FROM TOUCH DISPLAY:
    nextionLoop();

    //SWITCH MOTOR ON AND OFF:
    digitalWrite(MOTOR_RELAY_PIN, machineRunning);

    //GET SIGNAL FROM TEST SWITCH AND COUNT IT
    bool debouncedButtonState = testSwitch.requestButtonState();
    if (previousButtonState != debouncedButtonState) {
        if (debouncedButtonState == LOW) {
            switchCounter.countOneUp(longTimeCounter);
            updateDisplayCounter();
        }
        previousButtonState = debouncedButtonState;
    }


//runtime = millis() - runtimeStopwatch;
//Serial.println(runtime);
//runtimeStopwatch = millis();
}
