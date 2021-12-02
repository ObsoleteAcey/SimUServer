/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeLedHud.h"

#pragma region Constructors / Destructor

SimUServeLedHud::SimUServeLedHud()
{
  initDefaults(NUM_LEDS);

  this->_leds = new CRGB[NUM_LEDS];
}

SimUServeLedHud::SimUServeLedHud(uint8_t numberOfRevLeds)
{
  initDefaults(numberOfRevLeds);
  this->_numberOfRevLeds = numberOfRevLeds;
  this->_revLedStartIndex = 0;
  this->_revLedEndIndex = numberOfRevLeds - 1;
  // disable state LEDs
  this->_hasStateLeds = false;
  // re-evaluate the rev increment per LED.
  this->_revIncrementPerLed = 2 / numberOfRevLeds;

  this->_leds = new CRGB[numberOfRevLeds];
}

SimUServeLedHud::~SimUServeLedHud()
{
  delete[] this->_leds;
}

#pragma endregion

#pragma region Public methods

void SimUServeLedHud::updateLedState()
{
  this->_currentMillis = millis();

  if (this->_redLineReached)
  {
    this->displayRedline();
  }
  else
  {
    this->displayRpmLine();
  }

  this->flashFlag(CRGB::Yellow);
  FastLED.show();
}

void SimUServeLedHud::updateLedBrightness(uint8_t brightness)
{
  FastLED.setBrightness(brightness);
}

SimUServeLedHud *SimUServeLedHud::initLeds(void)
{
  FastLED.addLeds<LED_DEFAULT_TYPE, LED_DEFAULT_DATA_PIN, LED_DEFAULT_COLOUR_ORDER>(this->_leds, this->totalNumberOfLeds()).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(LED_DEFAULT_BRIGHTNESS);
  return this;
}

template <ESPIChipsets CHIPSET, uint8_t DATA_PIN, EOrder RGB_ORDER>
SimUServeLedHud *SimUServeLedHud::initLeds<ESPIChipsets, uint8_t, EOrder>(LEDColorCorrection colourCorrection, uint8_t brightness)
{
  FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>(this->_leds, this->totalNumberOfLeds()).setCorrection(colourCorrection);

  FastLED.setBrightness(brightness);

  return this;
}

void SimUServeLedHud::updateRpmLedState(double rpmState1, double rpmState2, bool redLineReached)
{
  this->_rpmShiftLight1 = rpmState1 >= 1 ? rpmState1 : 1;
  this->_rpmShiftLight2 = rpmState2 >= 1 ? rpmState2 : 1;
  this->_redLineReached = redLineReached;
}

#pragma endregion

#pragma region Protected methods

void SimUServeLedHud::displayRpmLine(void)
{

  int indexMax = rpmShiftToIndexLimit(this->_rpmShiftLight1, this->_rpmShiftLight2);

  if (indexMax == 0)
  {
    clearLeds(this->_revLedStartIndex, this->_revLedEndIndex);
    return;
  }

  // increment the max index as it's zero based, and we need
  // it to start from the first rev LED index
  indexMax += this->_revLedStartIndex;

  for (int index = this->_revLedStartIndex; index < indexMax; index++)
  {
    this->_leds[index] = indexToColour(index);
  }
}

void SimUServeLedHud::flashFlag(CRGB colour)
{
  // bail if no state LEDs, or the time expired to flash isn't up
  if (!this->_hasStateLeds || (this->_currentMillis - this->_previousFlagMillis < this->_flagCycleTime))
  {
    return;
  }

  for (uint8_t index = this->_firstStateLedStartIndex; index <= this->_lastStateLedEndIndex; index++)
  {
    this->_leds[index] = this->_flagState ? colour : CRGB::Black;
    if (index == this->_firstStateLedEndIndex)
    {
      index = this->_lastStateLedStartIndex - 1;
    }
  }

  this->_flagState = !this->_flagState;
  this->_previousFlagMillis = this->_currentMillis;
}

void SimUServeLedHud::displayRedline(void)
{
  if (this->_currentMillis - this->_previousRedlineMillis < this->_redlineCycleTime ||
      !this->_redLineReached)
  {
    return;
  }

  for (uint8_t i = this->_revLedStartIndex; i <= this->_revLedEndIndex; i++)
  {
    this->_leds[i] = this->_redlineState ? CRGB::Blue : CRGB::Black;
  }

  this->_previousRedlineMillis = this->_currentMillis;
  this->_redlineState = !_redlineState;
}

#pragma endregion

#pragma region Private methods

CRGB SimUServeLedHud::indexToColour(uint8_t index)
{

  if (index >= this->_revLedStartIndex && index < this->_revLedStartIndex + 6)
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
  return (uint8_t)((rpm1 + rpm2) / this->_revIncrementPerLed);
}

void SimUServeLedHud::clearLeds(uint8_t startIndex, uint8_t endIndex)
{
  for (uint8_t i = startIndex; i <= endIndex; i++)
  {
    this->_leds[i] = CRGB::Black;
  }
}

void SimUServeLedHud::initDefaults(uint8_t totalNumberOfLeds)
{
  // init flag cycle times
  this->_redlineCycleTime = REDLINE_CYCLE_TIME_MILLIS;
  this->_flagCycleTime = FLAG_CYCLE_TIME_MILLIS;
  // init the flash timers
  this->_previousFlagMillis = 0;
  this->_previousRedlineMillis = 0;
  this->_previousPitLimitMillis = 0;
  this->_previousLowFuelMillis = 0;
  // init the Rev LEDs
  this->_numberOfRevLeds = NUM_LEDS_REV;
  this->_revLedStartIndex = REV_DISPLAY_START_INDEX;
  this->_revLedEndIndex = REV_DISPLAY_END_INDEX;
  this->_revIncrementPerLed = 2 / NUM_LEDS_REV;
  // init the state LEDs
  this->_hasStateLeds = true;
  this->_firstStateLedStartIndex = FIRST_STATE_LED_START_INDEX;
  this->_firstStateLedEndIndex = FIRST_STATE_LED_END_INDEX;
  this->_lastStateLedStartIndex = LAST_STATE_LED_START_INDEX;
  this->_lastStateLedEndIndex = LAST_STATE_LED_END_INDEX;
}

uint8_t SimUServeLedHud::totalNumberOfLeds(void)
{
  uint8_t totalStateLeds = 0;
  // if we have state LEDs, lets total the number of them using their Index's
  if (this->_hasStateLeds)
  {
    totalStateLeds = (this->_firstStateLedEndIndex - this->_firstStateLedStartIndex + 1) +
                     (this->_lastStateLedEndIndex - this->_lastStateLedStartIndex + 1);
  }
  return this->_numberOfRevLeds + totalStateLeds;
}

#pragma endregion