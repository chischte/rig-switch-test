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
//*****************************************************************************
//DECLARATION OF VARIABLES / DATA TYPES
//*****************************************************************************
//boolean (true/false)
//byte (0-255)
//int   (-32,768 to 32,767) / unsigned int: 0 to 65,535
//long  (-2,147,483,648 to 2,147,483,647)
//float (6-7 Digits)
//*****************************************************************************

//KNOBS AND POTENTIOMETERS:
const byte TEST_SWITCH_PIN = 2;
const byte MOTOR_RELAY_PIN = 3;
//SENSORS:

//OTHER VARIABLES:
boolean currentButtonState;
boolean previousButtonState;
boolean machineRunning = false;
boolean stepMode = true;
boolean clearancePlayPauseToggle = true;
boolean clearanceNextStep = false;
boolean errorBlink = false;
boolean sealAvailable = false;

byte cycleStep = 1;
byte nexPrevCycleStep;

long upperFeedtime; //LONG because EEPROM function
long lowerFeedtime; //LONG because EEPROM function
long shorttimeCounter; //LONG because EEPROM function
long longtimeCounter; //LONG because EEPROM function

unsigned long runtime;
unsigned long runtimeStopwatch;
unsigned long prev_time;

Debounce testSwitch(TEST_SWITCH_PIN);

//*****************************************************************************
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup()
{
  Serial.begin(115200); //start serial connection

  nextionSetup();

 
  pinMode(MOTOR_RELAY, OUTPUT);

  Serial.println("EXIT SETUP");
}
//*****************************************************************************
//********************#*********#####***#####***######*************************
//********************#********#*****#*#*****#**#*****#************************
//********************#********#*****#*#*****#**######*************************
//********************#********#*****#*#*****#**#******************************
//********************#######***#####***#####***#******************************
//*****************************************************************************
void loop()
{

  digitalWrite(MOTOR_RELAY, !machineRunning);

runMainTestCycle();
NextionLoop();
EEPROM_Update();

  //runtime = millis() - runtimeStopwatch;
  //Serial.println(runtime);
  //runtimeStopwatch = millis();

}
