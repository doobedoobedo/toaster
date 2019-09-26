int plotY(int X) {
  int zone;
  int Y;
  unsigned long totalTime;
  unsigned long XTicks;

  totalTime = getTotalTime();

  //Serial.println(totalTime);

  XTicks = (totalTime * (unsigned long)X * 2UL) / 128UL;

  Y = 64 - ((getTargetTemp(XTicks) * 48) / 300) ;

  return Y;
}

unsigned long getTotalTime (void) {
  unsigned long tTime = 0UL;
  for (int i = 0; i < programmes[mainItem].numSteps; i++)
  {
    // get total time in seconds
    tTime +=  getStepTime(i);
  }

  return tTime;
}

unsigned long getStepTime (int i) {

  // get total time in seconds
  return  (programmes[mainItem].steps[i].rampTimeH * 3600) + (programmes[mainItem].steps[i].rampTimeM * 60) + programmes[mainItem].steps[i].rampTimeS;
}

int getTargetTemp(unsigned long t) {
  int temp = 0;
  // work out which step we're in
  for (int i = 0; i < programmes[mainItem].numSteps; i++)
  {
    if (stepTicks[i].ticksFrom <= t && stepTicks[i].ticksTo >= t)
    {
      if (i == 0)
      {
        temp = (int)(((unsigned long)(programmes[mainItem].steps[i].targetTemp - 20) * t) / (stepTicks[i].ticksTo - stepTicks[i].ticksFrom)) + 20UL;
      }
      else
      {
        if (programmes[mainItem].steps[i].targetTemp >= programmes[mainItem].steps[i - 1].targetTemp)
        {
          temp = (int)(((unsigned long)(programmes[mainItem].steps[i].targetTemp - programmes[mainItem].steps[i - 1].targetTemp) * (t - stepTicks[i].ticksFrom)) / (stepTicks[i].ticksTo - stepTicks[i].ticksFrom)) + (unsigned long)programmes[mainItem].steps[i - 1].targetTemp;
        }
        else
        {
          temp = (int)(unsigned long)programmes[mainItem].steps[i - 1].targetTemp - (((unsigned long)(programmes[mainItem].steps[i - 1].targetTemp - programmes[mainItem].steps[i].targetTemp) * (t - stepTicks[i].ticksFrom)) / (stepTicks[i].ticksTo - stepTicks[i].ticksFrom)) ;
        }
      }
    }
  }
  return temp;
}

void startRun(void) {
  runTime = M_0;
  unsigned long runningTotal = 0;
  unsigned long t;
  for (int i = 0; i < programmes[mainItem].numSteps; i++)
  {
    t = 2 * getStepTime(i);
    stepTicks[i].ticksFrom = runningTotal + 1;
    runningTotal += t;
    stepTicks[i].ticksTo = runningTotal;
  }
  for (int i = 0; i < 128; i++)
  {
    graphY[i] = plotY(i);
  }
  isRunning = true;
  //Serial.println("Started");

}

void stopRun(void) {
  isRunning = false;
  digitalWrite(relayPin, LOW);
  // bell????
  screen = MAIN;
  redraw = true;
  //Serial.println("Stopped");
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  // read temperature
  celcius = thermocouple.readCelsius();
  // calc target temp
  // switch heater on/off
  // increment runtime
  if (isRunning)
  {
    //Serial.print("runTime \t");
    //Serial.println(runTime);
    //Serial.print("ticksTo \t");
    //Serial.println(stepTicks[programmes[mainItem].numSteps -1].ticksTo);
    if (runTime > stepTicks[programmes[mainItem].numSteps - 1].ticksTo)
    {
      stopRun();
    }
    else
    {
      runTime++;
      targetTemp = getTargetTemp(runTime);

      if ((int) celcius > targetTemp)
      {
        digitalWrite(relayPin, LOW);
      }
      if ((int) celcius < targetTemp)
      {
        digitalWrite(relayPin, HIGH);
      }
    }
  }
  else
  {
    digitalWrite(relayPin, LOW);
  }

}
