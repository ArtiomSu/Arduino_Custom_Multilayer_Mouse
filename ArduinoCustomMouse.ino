//#include <Mouse.h>
#include "HID-Project.h"
#include "arduinoCustomMouse.h" //all the global variables
#define TESTING //Comment out to compile the prod code
#define USESERIAL //comment out to get rid of serial

//TODO: Toggle leds to show different modes.


/*
 There are 3 different modes currently
 1. normal 
 2. shifted : this is when the mode key is held down
 3. speacial: this is when the mode key is pressed
*/

/* Consumer codes in ~/Arduino/libraries/HID-Project/src/HID-APIs/ConsumerAPI.h
HID_CONSUMER_AC_SCROLL_UP = 0x233
HID_CONSUMER_AC_SCROLL_DOWN  = 0x234
CONSUMER_BROWSER_BACK = 0x224
CONSUMER_BROWSER_FORWARD = 0x225

write()
press()
release()
*/

/* Mouse codes in ~/Arduino/libraries/HID-Project/src/HID-APIs/AbsoluteMouseAPI.h
MOUSE_LEFT
MOUSE_RIGHT
MOUSE_MIDDLE
MOUSE_PREV
MOUSE_NEXT

click()
move()
press()
release()
*/

void setup()
{
  //buttons setip
  pinMode(mouseLeftClickPin, INPUT);
  pinMode(mouseRightClickPin, INPUT);
  pinMode(mouseMiddleClickPin, INPUT);
  pinMode(mouseModPin, INPUT);

  pinMode(modLEDPin, OUTPUT);

  //joystick setup 
  pinMode(horzPin, INPUT);  // Set both analog pins as inputs
  pinMode(vertPin, INPUT);
  pinMode(selPin, INPUT);  // set button select pin as input
  digitalWrite(selPin, HIGH);  // Pull button select pin high
  delay(1000);  // short delay to let outputs settle
  vertZero = analogRead(vertPin);  // get the initial values
  horzZero = analogRead(horzPin);  // Joystick should be in neutral position when reading these

  #ifdef USESERIAL
    #pragma message("Enabling Serial")
    Serial.begin(9600);
  #endif

  Mouse.begin(); //Init mouse emulation
  Consumer.begin(); //for special keys

  
}


void loop()
{
  joystick_read();
  //mouse_buttons_read();
  delay(1);
}
