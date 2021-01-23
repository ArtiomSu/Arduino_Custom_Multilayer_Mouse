#ifndef TESTING
#pragma message("########################################")
#pragma message("building from ArduinoCustomMouseProd.ino")
#pragma message("########################################")

void joystick_read(){
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
    if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
    {
      mouseClickFlag = 1;
      autoClickerHoldEnable = 0;
      autoClickerAFKEnable = 0;
      useSpecialMode = 0;
    }
    else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
    {
      mouseClickFlag = 0;
    }
    
//    #ifdef USESERIAL
//      Serial.print(vertValue);
//      Serial.print(" ");
//      Serial.print(horzValue);
//      Serial.print(" ");
//      Serial.print((vertValue / sensitivity));
//      Serial.print(" ");
//      Serial.print(invertMouse * (horzValue / sensitivity));
//      Serial.println("");
//    #endif
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
    if(leftClickPressed && (mouse_scroll_counter == 0)){
        mouseLeftClickFlag = 1;
        Mouse.move(0, 0, 1);
      }else if(!leftClickPressed && mouseLeftClickFlag){
        mouseLeftClickFlag = 0;
        //Mouse.release(MOUSE_LEFT);
      }
    
    
    if(rightClickPressed && (mouse_scroll_counter == 0)){
      mouseRightClickFlag = 1;
      Mouse.move(0, 0, -1);     
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

    mouse_scroll_counter++;
    if(mouse_scroll_counter > mouse_scroll_counter_delay){
      mouse_scroll_counter = 0;
    }
}


void mouse_buttons_read(){
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
