#ifndef ARDUINOCUSTOMMOUSE
#define ARDUINOCUSTOMMOUSE

/* Joystick Setup

GND   brown   GND

+5V   orange  VCC

VRx   red     A0

VRy   yellow  A1

SW    green   9


*/

int horzPin = A0;  // Analog output of horizontal joystick pin
int vertPin = A1;  // Analog output of vertical joystick pin
int selPin = 9;  // select button pin of joystick

int mouseLeftClickPin = 8;
int mouseRightClickPin = 7;
int mouseMiddleClickPin = 6;
int mouseModPin = 5;
int modLEDPin = 4;

int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
const int sensitivity = 20;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouse_slow_down_counter = 0;
const int mouse_slow_down_counter_delay = 10; // this will read joystick every 10th loop as other wise it is too fast and inacurate. joystick still feels smooth.

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
