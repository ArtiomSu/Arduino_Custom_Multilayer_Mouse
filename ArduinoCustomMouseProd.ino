#ifndef TESTING
#pragma message("########################################")
#pragma message("building from ArduinoCustomMouseProd.ino")
#pragma message("########################################")

void joystick_read(){
  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

  //use this for scrolling also maybe when mod key is pressed down
  if (vertValue != 0)
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
  if (horzValue != 0)
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis

  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_PREV);  // click the left button down
    //Consumer.write(CONSUMER_BROWSER_BACK);
  }
  else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_PREV);  // release the left button
  }
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

    //could me like enable auto clicker and toggle led to we know its active
    if(middleClickPressed && !mouseMiddleClickFlag){
      mouseMiddleClickFlag = 1;
      autoClickerHoldEnable = !autoClickerHoldEnable;
    }else if(!middleClickPressed && mouseMiddleClickFlag){
      mouseMiddleClickFlag = 0;
    }
}

void normal_mode(bool leftClickPressed, bool rightClickPressed, bool middleClickPressed){
    if(autoClickerHoldEnable){
      if(leftClickPressed){
        Mouse.click(MOUSE_LEFT);
      }
    }else{
      if(leftClickPressed && !mouseLeftClickFlag){
        mouseLeftClickFlag = 1;
        Mouse.press(MOUSE_LEFT);
      }else if(!leftClickPressed && mouseLeftClickFlag){
        mouseLeftClickFlag = 0;
        Mouse.release(MOUSE_LEFT);
      }
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

void special_mode(bool leftClickPressed, bool rightClickPressed, bool middleClickPressed){
    if(leftClickPressed && !mouseLeftClickFlag){
        mouseLeftClickFlag = 1;
        Consumer.write(HID_CONSUMER_AC_SCROLL_UP);
      }else if(!leftClickPressed && mouseLeftClickFlag){
        mouseLeftClickFlag = 0;
        //Mouse.release(MOUSE_LEFT);
      }
    
    
    if(rightClickPressed && !mouseRightClickFlag){
      mouseRightClickFlag = 1;
      Consumer.write(HID_CONSUMER_AC_SCROLL_DOWN);
    }else if(!rightClickPressed && mouseRightClickFlag){
      mouseRightClickFlag = 0;
      //Mouse.release(MOUSE_RIGHT);
    }

    if(middleClickPressed && !mouseMiddleClickFlag){
      mouseMiddleClickFlag = 1;
      autoClickerAFKEnable = !autoClickerAFKEnable;
    }else if(!middleClickPressed && mouseMiddleClickFlag){
      mouseMiddleClickFlag = 0;
    }
}

void mouse_buttons_read(){
  bool leftClickPressed = digitalRead(mouseLeftClickPin) == HIGH;
  bool rightClickPressed = digitalRead(mouseRightClickPin) == HIGH;
  bool middleClickPressed = digitalRead(mouseMiddleClickPin) == HIGH;
  bool modClickPressed = digitalRead(mouseModPin) == HIGH;

  
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
    Mouse.click(MOUSE_LEFT);
  }

  
}

#endif