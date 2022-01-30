# Arduino_Custom_Multilayer_Mouse
This is a custom mouse built using arduino pro micro, ps2 style joystick, 4 cherry mx switches.

Features a multilayer system similiar idea to qmk layers.

To see all features and a build/code guide go to my [website here](https://artiomsu.github.io/?cat=container_Projects&iteminner=null&catonly=0&item=container_arduinocustommouse)

# How it looks like
![Custom mouse](/screens/21.jpg)

![Custom mouse](/screens/20.jpg)

# (New) Extra mouse shaker mode
1. button1: start mouse jitter
2. button2: start keyboard afk

# Layout
To toggle between mouse jitter mode and mouse mode press the joystick button

## Mouse Jitter Mode
Mouse Move will randomly move the mouse around the screen. Keyboard Move will randomly press and or hold hold w,a,s,d,space  

| Button 1 | Button 2 | Button 3 | Button 4 |
|  :---:   |  :---:   |  :---:   |  :---:   |
| Mouse Move | Keyboard Move | LED 250ms | LED 300ms |

### Joystick
| Horizontal Forward | Horizontal Back | Vertical Down  | Vertical up |
|  :---:   |  :---:   |  :---:   |  :---:   |
| LED 50ms | LED 100ms | LED 150ms | LED 200ms |

## Mouse Mode
Joystick is used to move the mouse around. Mouse Mode has a few different layouts depending on the state of the MOD key. If you are familiar with QMK this is essentially layers.

### Normal Layout
This is the initial state of the mouse. Holding down MOD will switch to MOD key held layout. Pressing MOD will switch to MOD Layout

| Button 1 | Button 2 | Button 3 | Button 4 |
|  :---:   |  :---:   |  :---:   |  :---:   |
| Left Click | Right Click | Middle Click | MOD |

### MOD Key Held Layout
Pressing button 3 will toggle autoclicker hold. If enabled when holding button 1-3 on Normal Layout it will click many times a second. The Joystick in this layout is treated like extra buttons like in the mouse jitter mode.

| Button 1 | Button 2 | Button 3 | Button 4 |
|  :---:   |  :---:   |  :---:   |  :---:   |
| Back | Forward | AutoClicker Hold | |

#### Joystick
| Horizontal Forward | Horizontal Back | Vertical Down  | Vertical up |
|  :---:   |  :---:   |  :---:   |  :---:   |
| Scroll Up | Scroll Down | | |

### MOD Layout
AFK AutoClicker will constanctly left click until turned off. Pressing MOD will return to Normal Layout

| Button 1 | Button 2 | Button 3 | Button 4 |
|  :---:   |  :---:   |  :---:   |  :---:   |
| Increase Mouse Speed | Decrease Mouse Speed | AFK AutoClicker | MOD |