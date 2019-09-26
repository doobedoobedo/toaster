void debounce(byte i) {

  if (buttons[i].debouncing) {
    boolean currentPressed = digitalRead(buttons[i].pin);
    if (buttons[i].debouncePressed != currentPressed) {
      buttons[i].debouncing = false;
      buttons[i].debounceTime = millis();
    }
    buttons[i].debouncePressed = currentPressed;
  }

  if (buttons[i].debouncing == false) {
    unsigned long currentMillis = millis();
    if ( currentMillis - buttons[i].debounceTime >= DEBOUNCE ) {
      buttons[i].is_pressed = digitalRead(buttons[i].pin);
      buttons[i].debouncing = true;
    }
  }
}

void handleButtons(void) {
  byte i;
  unsigned long now;
  for (i = 0; i < NUMBUTTONS; i++) {
    debounce(i);
    if ( buttons[i].is_pressed != BPRESSED )
    {
      buttons[i].accelerator = 1;
    }
  }

  now = millis();

  switch (screen)
  {
    case MAIN:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        screen = STARTEDIT;
        redraw = true;
        if (programmes[mainItem].numSteps > 0)
        {
          seItem = 0;
        }
        else
        {
          seItem = 1;
        }
        buttons[BSELECT].is_pressed = !BPRESSED;
        //Serial.println("SELECT");
      }
      if (buttons[BUP].is_pressed == BPRESSED && mainItem > 0 )
      {
        mainItem--;
        redraw = true;
        buttons[BUP].is_pressed = !BPRESSED;
        //Serial.println("UP");
      }
      if (buttons[BDOWN].is_pressed == BPRESSED && mainItem < 2 )
      {
        mainItem++;
        redraw = true;
        buttons[BDOWN].is_pressed = !BPRESSED;
        //Serial.println("DOWN");
      }
      break;

    case STARTEDIT:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        switch (seItem)
        {
          case 0:
            screen = RUNNING;
            startRun();
            redraw = true;
            break;
          case 1:
            screen = SUBEDIT;
            redraw = true;
            break;
          case 2:
            screen = MAIN;
            redraw = true;
            break;
        }
        buttons[BSELECT].is_pressed = !BPRESSED;
        //Serial.println("SELECT");
      }

      if (buttons[BUP].is_pressed == BPRESSED && seItem > 0 )
      {
        if (programmes[mainItem].numSteps > 0)
        {
          seItem--;
          redraw = true;
        }
        else
        {
          if (seItem > 1)
          {
            seItem--;
            redraw = true;
          }
        }

        buttons[BUP].is_pressed = !BPRESSED;
        //Serial.println("UP");
      }

      if (buttons[BDOWN].is_pressed == BPRESSED && seItem < 2 )
      {
        seItem++;
        redraw = true;
        buttons[BDOWN].is_pressed = !BPRESSED;
        //Serial.println("DOWN");
      }
      break;

    case EDITNAME:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        // move to next character
        if (strItem < 13)
        {
          strItem++;
          curChar = strchr(characters, programmes[mainItem].progName[strItem]);
          redraw = true;
        }
        else
        {
          characters, programmes[mainItem].progName[14] = 0;
          EEPROM.put(eeAddress, programmes);
          screen = SUBEDIT;
          redraw = true;
        }
        //Serial.println(strItem);
        if ( buttons[BUP].accelerator < 10 )
        {
          buttons[BUP].accelerator++;
        }
        buttons[BSELECT].is_pressed = !BPRESSED;
        //Serial.println("SELECT");
      }

      if (buttons[BUP].is_pressed == BPRESSED && (millis() - lastAccelerate > M_1000 / buttons[BUP].accelerator || millis() - M_200 > lastRefresh))
      {
        lastAccelerate = millis();
        if (curChar - characters < 62)
        {
          curChar++;
          characters, programmes[mainItem].progName[strItem] = characters[curChar - characters];
          redraw = true;
        }
        else
        {
          curChar = strchr(characters, 'A');
          characters, programmes[mainItem].progName[strItem] = characters[curChar - characters];
          redraw = true;
        }
        if ( buttons[BUP].accelerator < 10 )
        {
          buttons[BUP].accelerator++;
        }
        //Serial.println("UP");
      }

      if (buttons[BDOWN].is_pressed == BPRESSED && (millis() - lastAccelerate > M_1000 / buttons[BDOWN].accelerator || millis() - M_200 > lastRefresh))
      {
        lastAccelerate = millis();
        if (curChar - characters > 0)
        {
          curChar--;
          characters, programmes[mainItem].progName[strItem] = characters[curChar - characters];
          redraw = true;
        }
        else
        {
          curChar = strchr(characters, '0');
          characters, programmes[mainItem].progName[strItem] = characters[curChar - characters];
          redraw = true;
        }
        if ( buttons[BDOWN].accelerator < 10 )
        {
          buttons[BDOWN].accelerator++;
        }
        //Serial.println("DOWN");
      }
      break;

    case SUBEDIT:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        switch (subItem)
        {
          case 0:
            screen = EDITNAME;
            redraw = true;
            strItem = 0;
            curChar = strchr(characters, programmes[mainItem].progName[0]);
            //Serial.println(characters[curChar-characters]);
            break;
          case 1:
            screen = EDITVALUES;
            valItem = 0;
            lineItem = 0;
            window = 0;
            flashing = true;
            /* set values from saved settings */

            redraw = true;
            break;
          case 2:
            screen = STARTEDIT;
            redraw = true;
            break;
        }
        buttons[BSELECT].is_pressed = !BPRESSED;
        //Serial.println("SELECT");
      }

      if (buttons[BUP].is_pressed == BPRESSED && subItem > 0 )
      {
        subItem--;
        redraw = true;

        buttons[BUP].is_pressed = !BPRESSED;
        //Serial.println("UP");
      }

      if (buttons[BDOWN].is_pressed == BPRESSED && subItem < 2 )
      {
        subItem++;
        redraw = true;
        buttons[BDOWN].is_pressed = !BPRESSED;
        //Serial.println("DOWN");
      }
      break;

    case EDITVALUES:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        if (valItem == 6)
        {

          for (int i = 0; i < 6; i++)
          {
            if (programmes[mainItem].steps[i].rampTimeH > 0 || programmes[mainItem].steps[i].rampTimeM > 0 || programmes[mainItem].steps[i].rampTimeS > 0)
            {
              programmes[mainItem].numSteps = i + 1;
            }
          }
          // return
          //Serial.println(programmes[mainItem].numSteps);
          EEPROM.put(eeAddress, programmes);
          screen = SUBEDIT;
          redraw = true;
        }
        else
        {
          flashing = !flashing;
        }

        buttons[BSELECT].is_pressed = !BPRESSED;
        //Serial.println("SELECT");
      }

      if (buttons[BUP].is_pressed == BPRESSED /*&& valItem > 0*/ && (millis() - lastAccelerate > M_1000 / buttons[BUP].accelerator || millis() - M_200 > lastRefresh))
      {
        lastAccelerate = millis();
        if (flashing)
        {
          switch (lineItem)
          {
            case 0: //temp
              if (programmes[mainItem].steps[valItem].targetTemp < 300)
              {
                programmes[mainItem].steps[valItem].targetTemp++;;
                redraw = true;
              }

              redraw = true;
              break;
            case 1: //hours
              if (programmes[mainItem].steps[valItem].rampTimeH < 48)
              {
                programmes[mainItem].steps[valItem].rampTimeH++;
                redraw = true;
              }
              break;
            case 2: //minutes
              if (programmes[mainItem].steps[valItem].rampTimeM < 59)
              {
                programmes[mainItem].steps[valItem].rampTimeM++;
                redraw = true;
              }
              break;
            case 3: //seconds
              if (programmes[mainItem].steps[valItem].rampTimeS < 59)
              {
                programmes[mainItem].steps[valItem].rampTimeS++;
                redraw = true;
              }
              break;
          }
          if ( buttons[BUP].accelerator < 30 )
          {
            buttons[BUP].accelerator++;
          }
        }
        else
        {
          switch (lineItem)
          {
            case 0: //temp
              if (valItem != 6)
              {
                lineItem++;
              }
              break;
            case 1: //hours
              lineItem++;
              break;
            case 2: //minutes
              lineItem++;
              break;
            case 3: //seconds
              if (valItem < 6 )
              {
                valItem++;
                lineItem = 0;
                if (valItem > window + 2)
                {
                  window++;
                }
              }
              break;
          }
        }

        redraw = true;

        //Serial.println("UP");
      }

      if (buttons[BDOWN].is_pressed == BPRESSED /*&& valItem < 2*/ && (millis() - lastAccelerate > M_1000 / buttons[BDOWN].accelerator || millis() - M_200 > lastRefresh))
      {
        lastAccelerate = millis();
        if (flashing)
        {
          switch (lineItem)
          {
            case 0: //temp
              if (programmes[mainItem].steps[valItem].targetTemp > 20)
              {
                programmes[mainItem].steps[valItem].targetTemp--;
                redraw = true;
              }
              break;
            case 1: //hours
              if (programmes[mainItem].steps[valItem].rampTimeH > 0)
              {
                programmes[mainItem].steps[valItem].rampTimeH--;
                redraw = true;
              }
              break;
            case 2: //minutes
              if (programmes[mainItem].steps[valItem].rampTimeM > 0)
              {
                programmes[mainItem].steps[valItem].rampTimeM--;
                redraw = true;
              }
              break;
            case 3: //seconds
              if (programmes[mainItem].steps[valItem].rampTimeS > 0)
              {
                programmes[mainItem].steps[valItem].rampTimeS--;
                redraw = true;
              }
              break;
          }
          if ( buttons[BDOWN].accelerator < 30 )
          {
            buttons[BDOWN].accelerator++;
          }
        }
        else
        {
          switch (lineItem)
          {
            case 0: //temp
              if (valItem > 0)
              {
                valItem--;
                lineItem = 3;
                if (valItem < window)
                {
                  window--;
                }
              }
              break;
            case 1: //hours
              lineItem--;
              break;
            case 2: //minutes
              lineItem--;
              break;
            case 3: //seconds
              lineItem--;
              break;

          }
        }
        redraw = true;
        //Serial.println("DOWN");
      }
      break;

    case RUNNING:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        screen = CANCEL;
        redraw = true;
        buttons[BSELECT].is_pressed = !BPRESSED;
      }
      break;

    case CANCEL:
      if (buttons[BSELECT].is_pressed == BPRESSED)
      {
        if (canItem == 0)
        {
          stopRun();
          redraw = true;
        }
        else
        {
          screen = RUNNING;
          redraw = true;
        }

        buttons[BSELECT].is_pressed = !BPRESSED;
      }

      if (buttons[BUP].is_pressed == BPRESSED && canItem > 0 )
      {
        canItem--;
        redraw = true;
        buttons[BUP].is_pressed = !BPRESSED;
        //Serial.println("UP");
      }
      if (buttons[BDOWN].is_pressed == BPRESSED && canItem < 1 )
      {
        canItem++;
        redraw = true;
        buttons[BDOWN].is_pressed = !BPRESSED;
        //Serial.println("DOWN");
      }
      break;
  }
}
