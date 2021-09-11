/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeLedHud.h"

SimUServeLedHud::SimUServeLedHud() 
{
  initDefaults(NUM_LEDS);

  this->leds = new CRGB[NUM_LEDS];
}

SimUServeLedHud::SimUServeLedHud(uint8_t numberOfRevLeds)
{
  initDefaults(numberOfRevLeds);
  this->numberOfRevLeds = numberOfRevLeds;
  this->revLedStartIndex = 0;
  this->revLedEndIndex = numberOfRevLeds - 1;
  // disable state LEDs
  this->hasStateLeds = false;
  // re-evaluate the rev increment per LED.
  this->revIncrementPerLed = 2/numberOfRevLeds;

  this->leds = new CRGB[numberOfRevLeds];
}

SimUServeLedHud::~SimUServeLedHud()
{
  delete[] this->leds;
}

CRGB SimUServeLedHud::indexToColour(uint8_t index) {

  if(index >= this->revLedStartIndex && index < this->revLedStartIndex + 6)
  {
    return CRGB::Green;
  }
  else if (index >= 9 && index < 14)
  {
    return CRGB::Red;
  }

  return CRGB::Blue;
}

uint8_t SimUServeLedHud::rpmShiftToIndexLimit(double rpm1, double rpm2)
{
  return (uint8_t)((rpm1+rpm2)/this->revIncrementPerLed);
}

void SimUServeLedHud::clearLeds(uint8_t startIndex, uint8_t endIndex) 
{
  for (uint8_t i = startIndex; i <= endIndex; i++) {
    leds[i] = CRGB::Black;
  }
}

void SimUServeLedHud::displayRedline(unsigned long currentMillis)
{
  if(currentMillis - previousRedlineMillis < redlineCycleTime)
  {
    return;
  }

  for (uint8_t i = this->revLedStartIndex; i <= this->revLedEndIndex; i++) {
    leds[i] = redlineState ? CRGB::Blue : CRGB::Black;
  }

  previousRedlineMillis = currentMillis;
  redlineState = !redlineState;
}

void SimUServeLedHud::flashFlag(CRGB colour, unsigned long currentMillis)
{
  // bail if no state LEDs, or the time expired to flash isn't up
  if (!this->hasStateLeds || (currentMillis - this->previousFlagMillis < this->flagCycleTime))
  {
    return;
  }
  
  for (uint8_t index = this->firstStateLedStartIndex; index <= this->lastStateLedEndIndex; index++)
  {
    leds[index] = flagState ? colour : CRGB::Black;
    if (index == this->firstStateLedEndIndex)
    {
      index = this->lastStateLedStartIndex - 1;
    }
  }

  this->flagState = !this->flagState;
  this->previousFlagMillis = currentMillis;
}

void SimUServeLedHud::displayRpmLine()
{

  int indexMax = rpmShiftToIndexLimit(this->rpmShiftLight1, this->rpmShiftLight2);

  if(indexMax == 0)
  {
    clearLeds(this->revLedStartIndex, this->revLedEndIndex);
    return;
  }

  // increment the max index as it's zero based, and we need
  // it to start from the first rev LED index
  indexMax += this->revLedStartIndex;

  for(int index = this->revLedStartIndex; index < indexMax; index++)
  {
    leds[index] = indexToColour(index);
  } 
}

void SimUServeLedHud::updateLedState()
{
  unsigned long currentMillis = millis();
  
  if(this->redLineReached) 
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

void SimUServeLedHud::initDefaults(uint8_t totalNumberOfLeds)
{
  // init flag cycle times
  this->redlineCycleTime = REDLINE_CYCLE_TIME_MILLIS;
  this->flagCycleTime = FLAG_CYCLE_TIME_MILLIS;
  // init the flash timers
  this->previousFlagMillis = 0;
  this->previousRedlineMillis = 0;
  this->previousPitLimitMillis = 0;
  this->previousLowFuelMillis = 0;
  // init the Rev LEDs
  this->numberOfRevLeds = NUM_LEDS_REV;
  this->revLedStartIndex = REV_DISPLAY_START_INDEX;
  this->revLedEndIndex = REV_DISPLAY_END_INDEX;
  this->revIncrementPerLed = 2/NUM_LEDS_REV; 
  // init the state LEDs
  this->hasStateLeds = true;
  this->firstStateLedStartIndex = FIRST_STATE_LED_START_INDEX;
  this->firstStateLedEndIndex = FIRST_STATE_LED_END_INDEX;
  this->lastStateLedStartIndex = LAST_STATE_LED_START_INDEX;
  this->lastStateLedEndIndex = LAST_STATE_LED_END_INDEX;
  this->colourOrder = COLOUR_ORDER;
  this->dataPin = DATA_PIN
}

SimUServeLedHud* SimUServeLedHud::setDataPin(uint8_t dataPin)
{
  this->dataPin = dataPin;
  return this;
}

SimUServeLedHud* SimUServeLedHud::initLeds(void)
{
  FastLED.addLeds<LED_TYPE, this->dataPin, this->colourOrder>(this->leds, this->totalNumberOfLeds()).setCorrection(TypicalLEDStrip);

  return this;
}

uint8_t SimUServeLedHud::totalNumberOfLeds(void)
{
  return this->numberOfRevLeds;
}
