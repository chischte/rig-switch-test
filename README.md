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
 * using Arduino Uno, Nano, etc.
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

