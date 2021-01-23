#ifdef TESTING
#pragma message("########################################")
#pragma message("building from ArduinoCustomMouseTest.ino")
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
      Mouse.press(MOUSE_LEFT);  // click the left button down
      //Consumer.write(CONSUMER_BROWSER_BACK);
    }
    else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
    {
      mouseClickFlag = 0;
      Mouse.release(MOUSE_LEFT);  // release the left button
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


  void mouse_buttons_read(){
    bool leftClickPressed = digitalRead(mouseLeftClickPin) == LOW;
    bool rightClickPressed = digitalRead(mouseRightClickPin) == LOW;
    bool middleClickPressed = digitalRead(mouseMiddleClickPin) == LOW;
    bool modClickPressed = digitalRead(mouseModPin) == LOW;


      if(leftClickPressed && !mouseLeftClickFlag){
          mouseLeftClickFlag = 1;
          Mouse.press(MOUSE_LEFT);
          #ifdef USESERIAL
          Serial.println("Left Click Pressed");
          #endif
        }else if(!leftClickPressed && mouseLeftClickFlag){
          mouseLeftClickFlag = 0;
          Mouse.release(MOUSE_LEFT);
          #ifdef USESERIAL
          Serial.println("Left Click released");
          #endif
        }
      
      
      if(rightClickPressed && !mouseRightClickFlag){
        mouseRightClickFlag = 1;
        Mouse.press(MOUSE_RIGHT);
        #ifdef USESERIAL
        Serial.println("RIGHT Click Pressed");
        #endif
      }else if(!rightClickPressed && mouseRightClickFlag){
        mouseRightClickFlag = 0;
        Mouse.release(MOUSE_RIGHT);
        #ifdef USESERIAL
        Serial.println("RIGHT Click released");
        #endif
      }

      if(middleClickPressed && !mouseMiddleClickFlag){
        mouseMiddleClickFlag = 1;
        Mouse.press(MOUSE_MIDDLE);
        #ifdef USESERIAL
        Serial.println("middle Click Pressed");
        #endif
      }else if(!middleClickPressed && mouseMiddleClickFlag){
        mouseMiddleClickFlag = 0;
        Mouse.release(MOUSE_MIDDLE);
        #ifdef USESERIAL
        Serial.println("middle Click released");
        #endif
      }


    if(modClickPressed && !mouseModClickFlag){
        mouseModClickFlag = 1;
        lastTimeModClicked = millis();
        #ifdef USESERIAL
        Serial.println("mod Click Pressed");
        #endif
    }else if(!modClickPressed && mouseModClickFlag){
        mouseModClickFlag = 0;
        unsigned long now = millis();
        #ifdef USESERIAL
        Serial.println("mod Click released");
        #endif
        if((now - lastTimeModClicked) < 200){
          //this was a click not a hold since button was released after 200ms maybe decrease this later
          #ifdef USESERIAL
          Serial.println("mod Click clicked into next mode");
          #endif
        }
    }



    
  }

#endif
