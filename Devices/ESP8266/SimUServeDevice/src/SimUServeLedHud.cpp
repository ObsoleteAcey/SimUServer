#include "SimUServeLedHud.h"
#include "SimUServeCommon.h"

SimUServeLedHud::SimUServeLedHud() 
{
    
    initDefaults();
}

SimUServeLedHud::~SimUServeLedHud()
{

}

CRGB SimUServeLedHud::indexToColour(int index) {
  if(index >= 3 && index < 9)
  {
    return CRGB::Green;
  }
  else if (index >= 9 && index < 14)
  {
    return CRGB::Red;
  }

  return CRGB::Blue;
}

int SimUServeLedHud::rpmShiftToIndexLimit(double rpm1, double rpm2)
{
  int index = (int)((rpm1+rpm2)/0.125);
  return index;
}

void SimUServeLedHud::clearLeds(int startIndex, int endIndex) 
{
  for (int i = startIndex; i <= endIndex; i++) {
    leds[i] = CRGB::Black;
  }
}

void SimUServeLedHud::displayRedline(unsigned long currentMillis)
{
  if(currentMillis - previousRedlineMillis < redlineCycleTime)
  {
    return;
  }

  for (int i = REV_DISPLAY_START_INDEX; i <= REV_DISPLAY_END_INDEX; i++) {
    leds[i] = redlineState ? CRGB::Blue : CRGB::Black;
  }

  previousRedlineMillis = currentMillis;
  redlineState = !redlineState;
}

void SimUServeLedHud::flashFlag(CRGB colour, unsigned long currentMillis)
{
  if(currentMillis - previousFlagMillis < flagCycleTime)
  {
    return;
  }
  
  for(int index = FIRST_STATE_LED_START_INDEX; index <= LAST_STATE_LED_END_INDEX; index++)
  {
    leds[index] = flagState ? colour : CRGB::Black;
    if(index == FIRST_STATE_LED_END_INDEX)
    {
      index = LAST_STATE_LED_START_INDEX - 1;
    }
  }

  flagState = !flagState;
  previousFlagMillis = currentMillis;
}

void SimUServeLedHud::displayRpmLine()
{

  int indexMax = rpmShiftToIndexLimit(rpmShiftLight1, rpmShiftLight2);

  if(indexMax == 0)
  {
    clearLeds(REV_DISPLAY_START_INDEX, REV_DISPLAY_END_INDEX);
    return;
  }

  indexMax+=3;

  for(int index = 3; index < indexMax; index++)
  {
    leds[index] = indexToColour(index);
  } 
}

void SimUServeLedHud::updateLedState()
{
  unsigned long currentMillis = millis();
  
  if(redLineReached) 
  {
    displayRedline(currentMillis);
  }
  else
  {
    displayRpmLine();
  }

  flashFlag(CRGB::Yellow, currentMillis);
  FastLED.show();
}

SimUServeLedHud simUServeLedHud;