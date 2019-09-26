void drawSplash(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(46, 20);
  u8g.print("Ron's");
  u8g.setPrintPos(22, 35);
  u8g.print("Toaster Oven");
  u8g.setPrintPos(29, 50);
  u8g.print("Controller");
}

void drawStartedit(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(8, 12);
  u8g.print(programmes[mainItem].progName);
  if (programmes[mainItem].numSteps > 0)
  {
    u8g.setPrintPos(8, 30);
    u8g.print("Start");
  }
  u8g.setPrintPos(8, 45);
  u8g.print("Edit");
  u8g.setPrintPos(8, 60);
  u8g.print("Return");
  u8g.drawRFrame(5, 3 + (15 * (seItem + 1)), 101, 16, 3);

}

void drawMain(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(8, 12);
  u8g.print("Select Programme");
  u8g.setPrintPos(8, 30);
  u8g.print(programmes[0].progName);
  u8g.setPrintPos(8, 45);
  u8g.print(programmes[1].progName);
  u8g.setPrintPos(8, 60);
  u8g.print(programmes[2].progName);
  u8g.drawRFrame(5, 3 + (15 * (mainItem + 1)), 101, 16, 3);
  //u8g.setPrintPos(20, 64);
  //u8g.print("C = ");
  //u8g.setPrintPos(48, 64);
  //u8g.print(celcius);
}

void drawSubEdit(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(8, 12);
  u8g.print("Edit");
  u8g.setPrintPos(8, 30);
  u8g.print("Name");
  u8g.setPrintPos(8, 45);
  u8g.print("Programme");
  u8g.setPrintPos(8, 60);
  u8g.print("Return");
  u8g.drawRFrame(5, 3 + (15 * (subItem + 1)), 101, 16, 3);
}

void drawEditName(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos( 10, 30);
  u8g.print(programmes[mainItem].progName);
  u8g.drawRFrame(8 + (strItem * 7), 18 , 10, 16, 3);
}


void drawEditValues(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(8, 12);
  u8g.print("Temp");
  u8g.setPrintPos(64, 12);
  u8g.print("Ramp time");

  for (int i = 0; i < 7; i++)
  {
    if (30 <= ((i + 2) * 15) - (15 * window) && ((i + 2) * 15) - (15 * window) <= 60)
    {
      if (i < 6)
      {
        if (programmes[mainItem].steps[i].targetTemp < 100)
        {
          u8g.setPrintPos(10, ((i + 2) * 15) - (15 * window));
          u8g.print("0");
          u8g.setPrintPos(17, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].targetTemp);
        }
        else
        {
          u8g.setPrintPos(10, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].targetTemp);
        }
        if (programmes[mainItem].steps[i].rampTimeH < 10)
        {
          u8g.setPrintPos(64, ((i + 2) * 15) - (15 * window));
          u8g.print("0");
          u8g.setPrintPos(71, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].rampTimeH);
        }
        else
        {
          u8g.setPrintPos(64, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].rampTimeH);
        }
        u8g.setPrintPos(78, ((i + 2) * 15) - (15 * window));
        u8g.print(":");
        if (programmes[mainItem].steps[i].rampTimeM < 10)
        {
          u8g.setPrintPos(85, ((i + 2) * 15) - (15 * window));
          u8g.print("0");
          u8g.setPrintPos(92, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].rampTimeM);
        }
        else
        {
          u8g.setPrintPos(85, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].rampTimeM);
        }
        u8g.setPrintPos(99, ((i + 2) * 15) - (15 * window));
        u8g.print(":");
        if (programmes[mainItem].steps[i].rampTimeS < 10)
        {
          u8g.setPrintPos(106, ((i + 2) * 15) - (15 * window));
          u8g.print("0");
          u8g.setPrintPos(113, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].rampTimeS);
        }
        else
        {
          u8g.setPrintPos(106, ((i + 2) * 15) - (15 * window));
          u8g.print(programmes[mainItem].steps[i].rampTimeS);
        }
      }
      else
      {
        u8g.setPrintPos(50, ((i + 2) * 15) - (15 * window));
        u8g.print("Done");
      }
    }
  }


  if (!flashing || flash) {
    if (valItem < 6)
    {
      switch (lineItem)
      {
        case 0:
          u8g.drawRFrame(8, 3 + ((valItem + 1) * 15) - (15 * window), 24, 16, 3);
          break;
        case 1:
          u8g.drawRFrame(62, 3 + ((valItem + 1) * 15) - (15 * window), 17, 16, 3);
          break;
        case 2:
          u8g.drawRFrame(83, 3 + ((valItem + 1) * 15) - (15 * window), 17, 16, 3);
          break;
        case 3:
          u8g.drawRFrame(104, 3 + ((valItem + 1) * 15) - (15 * window), 17, 16, 3);
          break;
      }
    }
    else
    {
      u8g.drawRFrame(48, 3 + ((valItem + 1) * 15) - (15 * window), 31, 16, 3);
    }
  }
}

void drawRunning(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(0, 12);
  u8g.print("T");
  if (celcius < 100)
  {
    u8g.setPrintPos(17, 12);
  }
  else
  {
    u8g.setPrintPos(24, 12);
  }
  u8g.print((int)celcius);

  u8g.setPrintPos(44, 12);
  u8g.print("Tget");

  if (targetTemp >= 100)
  {
    u8g.setPrintPos(77, 12);
  }
  else
  {
    u8g.setPrintPos(84, 12);
  }
  u8g.print(targetTemp);
  // plot the graph
  for (int i = 0; i < 128; i++)
  {
    u8g.drawPixel(i, graphY[i]);
  }
  int x = (int)((float)runTime / ((float)stepTicks[programmes[mainItem].numSteps - 1].ticksTo / 128.0));

  u8g.drawLine(x, 16, x, 64);
}

void drawCancel(void) {
  u8g.setFont(u8g_font_7x13Br);
  u8g.setPrintPos(8, 12);
  u8g.print("Cancel");
  u8g.setPrintPos(8, 30);
  u8g.print("Yes");
  u8g.setPrintPos(8, 45);
  u8g.print("No");
  u8g.drawRFrame(5, 3 + (15 * (canItem + 1)), 101, 16, 3);
}

void drawScreen(void) {

  if ( redraw )
  {
    u8g.firstPage();
    do
    {
      switch (screen)
      {
        case SPLASH:
          drawSplash();
          break;
        case STARTEDIT:
          drawStartedit();
          break;
        case MAIN:
          drawMain();
          break;
        case SUBEDIT:
          drawSubEdit();
          break;
        case EDITNAME:
          drawEditName();
          break;
        case EDITVALUES:
          drawEditValues();
          break;
        case RUNNING:
          drawRunning();
          break;
        case CANCEL:
          drawCancel();
          break;
      }
    } while ( u8g.nextPage() );
    lastRefresh = millis();
  }

  switch (screen)
  {
    case SPLASH:
      screen = MAIN;
      redraw = true;
      delay(M_2500);
      break;
    case STARTEDIT:
    case MAIN:
    case SUBEDIT:
    case EDITNAME:
    case EDITVALUES:
    case RUNNING:
    case CANCEL:
      redraw = false;
      break;
  }
}
