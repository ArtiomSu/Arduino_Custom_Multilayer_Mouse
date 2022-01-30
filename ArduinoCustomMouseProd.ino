#ifndef TESTING
#pragma message("########################################")
#pragma message("building from ArduinoCustomMouseProd.ino")
#pragma message("########################################")

// #include "arduinoCustomMouse.h" // uncomment for vscode
/*
  MOUSE SHAKER FUNCTIONS START
*/

void empty_button(int delayTime){
  digitalWrite(modLEDPin, HIGH);
  delay(delayTime);
  digitalWrite(modLEDPin, LOW);
}

void turn_on_led(){
  if(runLED){
    led_counter++;
    if(led_counter > led_counter_delay){
      digitalWrite(modLEDPin, HIGH);
      led_counter = 0;
    }else{
      digitalWrite(modLEDPin, LOW);      
    }
  }
}

void jitter_mouse(){
  if(mouseJitter.move_mouse){
    mouseJitter.mouse_jitter_delay_counter++;
    if(mouseJitter.mouse_jitter_delay_counter > 10){
      mouseJitter.mouse_jitter_delay_counter = 0;
      mouseJitter.mouse_jitter_counter++;
      if(mouseJitter.mouse_jitter_counter > 500){
        mouseJitter.mouse_jitter_random_x = random(-1, 3);
        mouseJitter.mouse_jitter_random_y = random(-1, 3);
        if(mouseJitter.mouse_jitter_counter > 1000){
          mouseJitter.mouse_jitter_counter = 0;
        }
      }else{
        mouseJitter.mouse_jitter_random_x = random(-2, 2);
        mouseJitter.mouse_jitter_random_y = random(-2, 2);
      }
      Mouse.move(mouseJitter.mouse_jitter_random_x,mouseJitter.mouse_jitter_random_y,0);
      }
  }
}

void keyboard_afk(){
  if(keyboardAfk.keyboard_move){
    keyboardAfk.keyboard_counter++;
    if(keyboardAfk.keyboard_counter < random(500,2000)){
      if(!keyboardAfk.keyboard_pressed){
        Keyboard.press(keyboardAfk.keyboard_pattern[random(0, sizeof(keyboardAfk.keyboard_pattern)/sizeof(keyboardAfk.keyboard_pattern[0]))]);
        keyboardAfk.keyboard_pressed = 1;
        keyboardAfk.keyboard_released = 0;
      }
    }else{
      keyboardAfk.keyboard_counter = 0;
      Keyboard.releaseAll();
      keyboardAfk.keyboard_pressed = 0;
      keyboardAfk.keyboard_released = 1;
    }  
  }else{
    if(!keyboardAfk.keyboard_released){
      keyboardAfk.keyboard_released = 1;
      keyboardAfk.keyboard_pressed = 0;
      Keyboard.releaseAll();
    }
  }
}

void execute_routines(){
  jitter_mouse();
  keyboard_afk();
  turn_on_led();
}

void mouseShaker_buttons_read(){
  bool button1ClickPressed = digitalRead(mouseLeftClickPin) == LOW;
  bool button2ClickPressed = digitalRead(mouseRightClickPin) == LOW;
  bool button3ClickPressed = digitalRead(mouseMiddleClickPin) == LOW;
  bool button4ClickPressed = digitalRead(mouseModPin) == LOW;
  //bool joystickPressed = digitalRead(selPin) == LOW;

  if(button1ClickPressed && !button1ClickFlag){
    button1ClickFlag = 1;
    mouseJitter.move_mouse = !mouseJitter.move_mouse;
    runLED = mouseJitter.move_mouse;
    if(mouseJitter.move_mouse){
      led_counter_delay = 50;
    }else{
      digitalWrite(modLEDPin, LOW);
    }
  }else if(!button1ClickPressed && button1ClickFlag){
    button1ClickFlag = 0;
  }

  if(button2ClickPressed && !button2ClickFlag){
    button2ClickFlag = 1;
    keyboardAfk.keyboard_move = !keyboardAfk.keyboard_move;
    runLED = keyboardAfk.keyboard_move;
    if(keyboardAfk.keyboard_move){
      led_counter_delay = 100;
    }else{
      digitalWrite(modLEDPin, LOW);
    }
  }else if(!button2ClickPressed && button2ClickFlag){
    button2ClickFlag = 0;
  }

  if(button3ClickPressed && !button3ClickFlag){
    button3ClickFlag = 1;
    empty_button(250);
  }else if(!button3ClickPressed && button3ClickFlag){
    button3ClickFlag = 0;
  }

  if(button4ClickPressed && !button4ClickFlag){
    button4ClickFlag = 1;
    empty_button(300);
  }else if(!button4ClickPressed && button4ClickFlag){
    button4ClickFlag = 0;
  }

  // if (joystickPressed && !joystickClickFlag){
  //   joystickClickFlag = 1;
  //   empty_button(350);
  // }else if (joystickPressed && joystickClickFlag){
  //   joystickClickFlag = 0;
  // }
  execute_routines();
 
}

void joystick_read_as_buttons(void hf_p(void), void hf_r(void),
                               void hb_p(void), void hb_r(void),
                               void vd_p(void), void vd_r(void),
                               void vu_p(void), void vu_r(void),
                               bool continuous_mode){
  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

  int horizontal_forward_pressed = 0, horizontal_back_pressed = 0, vertical_down_pressed = 0, vertical_up_pressed = 0;

  if      (horzValue > 500) { horizontal_forward_pressed = 1;
  }else if(horzValue < -500){ horizontal_back_pressed = 1;
  }else if(vertValue < -490){ vertical_down_pressed = 1;
  }else if(vertValue > 500) { vertical_up_pressed = 1;
  }

  if(       horizontal_forward_pressed && (!horizontal_forward_pressed_flag || continuous_mode)){  horizontal_forward_pressed_flag = 1; hf_p();
  }else if(!horizontal_forward_pressed && horizontal_forward_pressed_flag){   horizontal_forward_pressed_flag = 0; hf_r();
  }

  if(       horizontal_back_pressed && (!horizontal_back_pressed_flag || continuous_mode)){        horizontal_back_pressed_flag = 1; hb_p();
  }else if(!horizontal_back_pressed && horizontal_back_pressed_flag){         horizontal_back_pressed_flag = 0; hb_r();
  }

  if(       vertical_down_pressed && (!vertical_down_pressed_flag || continuous_mode)){            vertical_down_pressed_flag = 1; vd_p();
  }else if(!vertical_down_pressed && vertical_down_pressed_flag){             vertical_down_pressed_flag = 0; vd_r();
  }

  if(       vertical_up_pressed && (!vertical_up_pressed_flag || continuous_mode)){                vertical_up_pressed_flag = 1; vu_p();
  }else if(!vertical_up_pressed && vertical_up_pressed_flag){                 vertical_up_pressed_flag = 0; vu_r();
  }
}

void mouseShaker_joystick_read(){
  auto empty = [](void) -> void {};
  auto led_50 = [](void) -> void { empty_button(50); };
  auto led_100 = [](void) -> void { empty_button(100); };
  auto led_150 = [](void) -> void { empty_button(150); };
  auto led_200 = [](void) -> void { empty_button(200); };

  joystick_read_as_buttons(
    led_50, empty,
    led_100, empty,
    led_150, empty,
    led_200, empty,
    false
  );
}

/*
  MOUSE SHAKER FUNCTIONS END
*/

void modHeldMode_joystick_read(){
  auto empty = [](void) -> void {};
  auto scroll_up = [](void) -> void { 
    if(mouse_scroll_counter == 0){
        Mouse.move(0, 0, 1);
    }
  };
  auto scroll_down = [](void) -> void { 
    if(mouse_scroll_counter == 0){
        Mouse.move(0, 0, -1);
    }
  };

  joystick_read_as_buttons(
    scroll_up, empty,
    scroll_down, empty,
    empty, empty,
    empty, empty,
    true
  );

  mouse_scroll_counter++;
  if(mouse_scroll_counter > mouse_scroll_counter_delay){
    mouse_scroll_counter = 0;
  }
}

void joystick_read(){

  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    mouseShakerMode = !mouseShakerMode;
  }
  else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
  }


  if(mouseShakerMode){
    mouseShaker_joystick_read();
    return;
  }

  if(mouseModClickFlag){
    modHeldMode_joystick_read();
    return;
  }

  if(mouse_slow_down_counter > mouse_slow_down_counter_delay){
  mouse_slow_down_counter = 0;
  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset


  //use this for scrolling also maybe when mod key is pressed down
  if (vertValue != 0)
    //Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
    Mouse.move(((vertValue / sensitivity)), 0, 0);
  if (horzValue != 0)
    //Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis
    Mouse.move(0, (invertMouse * (horzValue / sensitivity)), 0); // move mouse on y axis
  // if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  // {
  //   mouseClickFlag = 1;
  //   autoClickerHoldEnable = 0;
  //   autoClickerAFKEnable = 0;
  //   useSpecialMode = 0;
  // }
  // else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
  // {
  //   mouseClickFlag = 0;
  // }
  
  }
  mouse_slow_down_counter++;
}

void mod_key_held_down(bool leftClickPressed, bool rightClickPressed, bool middleClickPressed){
  if(leftClickPressed && !mouseLeftClickFlag){
    mouseLeftClickFlag = 1;
    Mouse.press(MOUSE_PREV);
  }else if(!leftClickPressed && mouseLeftClickFlag){
    mouseLeftClickFlag = 0;
    Mouse.release(MOUSE_PREV);
  }

  if(rightClickPressed && !mouseRightClickFlag){
    mouseRightClickFlag = 1;
    Mouse.press(MOUSE_NEXT);
  }else if(!rightClickPressed && mouseRightClickFlag){
    mouseRightClickFlag = 0;
    Mouse.release(MOUSE_NEXT);
  }

  if(middleClickPressed && !mouseMiddleClickFlag){
    mouseMiddleClickFlag = 1;
    autoClickerHoldEnable = !autoClickerHoldEnable;
  }else if(!middleClickPressed && mouseMiddleClickFlag){
    mouseMiddleClickFlag = 0;
  }
}

void normal_mode(bool leftClickPressed, bool rightClickPressed, bool middleClickPressed){
  if(autoClickerHoldEnable){
    if(afk_clicker_counter == 0){
      if(leftClickPressed){
        Mouse.click(MOUSE_LEFT);
      }

      if(rightClickPressed){
        Mouse.click(MOUSE_RIGHT);
      }

      if(middleClickPressed){
        Mouse.click(MOUSE_MIDDLE);
      }        
      
    }
  }else{
    if(leftClickPressed && !mouseLeftClickFlag){
      mouseLeftClickFlag = 1;
      Mouse.press(MOUSE_LEFT);
    }else if(!leftClickPressed && mouseLeftClickFlag){
      mouseLeftClickFlag = 0;
      Mouse.release(MOUSE_LEFT);
    }

    if(rightClickPressed && !mouseRightClickFlag){
      mouseRightClickFlag = 1;
      Mouse.press(MOUSE_RIGHT);
    }else if(!rightClickPressed && mouseRightClickFlag){
      mouseRightClickFlag = 0;
      Mouse.release(MOUSE_RIGHT);
    }

    if(middleClickPressed && !mouseMiddleClickFlag){
      mouseMiddleClickFlag = 1;
      Mouse.press(MOUSE_MIDDLE);
    }else if(!middleClickPressed && mouseMiddleClickFlag){
      mouseMiddleClickFlag = 0;
      Mouse.release(MOUSE_MIDDLE);
    }
  }
}

void special_mode(bool leftClickPressed, bool rightClickPressed, bool middleClickPressed){

  if(leftClickPressed && !mouseLeftClickFlag){
    mouseLeftClickFlag = 1;
    if(sensitivity > 5){
      sensitivity--;
    }
    if(sensitivity > 10){
      sensitivity-=3;
    }
  }else if(!leftClickPressed && mouseLeftClickFlag){
    mouseLeftClickFlag = 0;
  }
  
  if(rightClickPressed && !mouseRightClickFlag){
    mouseRightClickFlag = 1;
    if(sensitivity < 40){
      sensitivity++;
      if(sensitivity > 10){
        sensitivity+=3;
      }
    }
  }else if(!rightClickPressed && mouseRightClickFlag){
    mouseRightClickFlag = 0;
  }

  if(middleClickPressed && !mouseMiddleClickFlag){
    mouseMiddleClickFlag = 1;
    autoClickerAFKEnable = !autoClickerAFKEnable;
  }else if(!middleClickPressed && mouseMiddleClickFlag){
    mouseMiddleClickFlag = 0;
  }

  mouse_scroll_counter++;
  if(mouse_scroll_counter > mouse_scroll_counter_delay){
    mouse_scroll_counter = 0;
  }
}


void mouse_buttons_read(){

  if(mouseShakerMode){
    mouseShaker_buttons_read();
    return;
  }

  led_counter_delay = 100;
  bool leftClickPressed = digitalRead(mouseLeftClickPin) == LOW;
  bool rightClickPressed = digitalRead(mouseRightClickPin) == LOW;
  bool middleClickPressed = digitalRead(mouseMiddleClickPin) == LOW;
  bool modClickPressed = digitalRead(mouseModPin) == LOW;

  
  if(modClickPressed){ //this will be like if the mod key is held down.
    mod_key_held_down(leftClickPressed,rightClickPressed,middleClickPressed);
  }else{ //here the mod key is released
    //here it can go into normal mode or the special mode
    if(useSpecialMode){
      special_mode(leftClickPressed,rightClickPressed,middleClickPressed);
    }else{
      normal_mode(leftClickPressed,rightClickPressed,middleClickPressed);
    }
  }

  if(modClickPressed && !mouseModClickFlag){
      mouseModClickFlag = 1;
      lastTimeModClicked = millis();
  }else if(!modClickPressed && mouseModClickFlag){
      mouseModClickFlag = 0;
      unsigned long now = millis();
      if((now - lastTimeModClicked) < 200){
        //this was a click not a hold since button was released after 200ms maybe decrease this later
        useSpecialMode = !useSpecialMode; //toggle special mode
      }
  }

  if(autoClickerAFKEnable){
    if(afk_clicker_counter == 0){
      Mouse.click(MOUSE_LEFT);
    }
  }

  //same counter for all auto clickers
  afk_clicker_counter++;
  if(afk_clicker_counter > afk_clicker_counter_delay){
    afk_clicker_counter = 0;
  }

  if(useSpecialMode){
    led_counter++;
    if(led_counter > led_counter_delay/2){
      digitalWrite(modLEDPin, HIGH);
      led_counter = 0;
    }else{
      digitalWrite(modLEDPin, LOW);      
    }
  }else{
    digitalWrite(modLEDPin, LOW);
  }

  
}

#endif
