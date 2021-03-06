#ifndef ARDUINOCUSTOMMOUSE
#define ARDUINOCUSTOMMOUSE

/* Joystick Setup

GND   brown   GND

+5V   orange  VCC

VRx   red     A0

VRy   yellow  A1

SW    green   9


*/


//pins
int horzPin = A0;  // Analog output of horizontal joystick pin
int vertPin = A1;  // Analog output of vertical joystick pin
int selPin = 9;  // select button pin of joystick

int mouseLeftClickPin = 7;
int mouseRightClickPin = 8;
int mouseMiddleClickPin = 6;
int mouseModPin = 5;
int modLEDPin = 4;

//config settings (change these to whatever you want)
const int sensitivity = 10;  // Higher sensitivity value = slower mouse. Joystick sensitivity
const int mouse_slow_down_counter_delay = 20; // this will read joystick every 20th loop as other wise it is too fast and inacurate. joystick still feels smooth.
const int mouse_scroll_counter_delay = 20; //scroll speed. The higher the value the slower the scroll will be
const int afk_clicker_counter_delay = 15; //increase to slow down the auto clicker. min=15 anything lower and system will freeze.
const int led_counter_delay = 100; //How fast the led will blink when special mode is active. higher number = slower blink


// internal values (do not change)
int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis

int mouse_slow_down_counter = 0;
int mouse_scroll_counter = 0;
int afk_clicker_counter = 0;
int led_counter = 0;

//button flags
int mouseClickFlag = 0;
int mouseLeftClickFlag = 0;
int mouseRightClickFlag = 0;
int mouseMiddleClickFlag = 0;
int mouseModClickFlag = 0;

//auto clicker
int autoClickerHoldEnable = 0;
int autoClickerAFKEnable = 0;

//normal mode or special mode values;
unsigned long lastTimeModClicked;
int useSpecialMode = 0;

//int invertMouse = 1;        //Invert joystick based on orientation
int invertMouse = -1;         //Noninverted joystick based on orientation

#endif
