#include "display.h"
#include <Arduino.h>

// Constructor
Display::Display(){

}

void Display::new_send_to_nextion() {
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}