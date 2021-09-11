/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#ifndef SimUServeLedHud_h
#define SimUServeLedHud_h

#include "SimUServeCommon.h"
#include <FastLED.h>
#include <stdint.h>

// default settings
#define NUM_LEDS 22
#define NUM_LEDS_REV 16
#define LED_TYPE WS2812B
#define COLOUR_ORDER GRB
#define FIRST_STATE_LED_START_INDEX 0
#define FIRST_STATE_LED_END_INDEX 2
#define LAST_STATE_LED_START_INDEX 19
#define LAST_STATE_LED_END_INDEX 21
#define REV_DISPLAY_START_INDEX 3
#define REV_DISPLAY_END_INDEX 18
#define FLAG_CYCLE_TIME_MILLIS 500
#define REDLINE_CYCLE_TIME_MILLIS 100

// these come from SimHub.  The first 2 are between 0 and 1 and represent the 2 zones
// for 16 LEDs, we'll go [G,G,G,G,G,G,R,R,R,R,R,B,B,B,B,B]
//                       |--    rpm1   --|--    rpm2   --|
// this means 0.125 per LED
class SimUServeLedHud {
  protected:

  
  private:
    double rpmShiftLight1;
    double rpmShiftLight2;
    double redLineReached;
    bool flagState;
    bool redlineState;
    bool pitLimitState;
    bool lowFuelState;

    // timer state attributes
    uint16_t previousFlagMillis;
    uint16_t previousRedlineMillis;
    uint16_t previousPitLimitMillis;
    uint16_t previousLowFuelMillis;
    uint16_t flagCycleTime;
    uint16_t redlineCycleTime;
    /*---------- LED config settings -------------*/
    // make sure this is initilised only once
    CRGB* leds;
    // Rev LED config
    uint8_t numberOfRevLeds;
    uint8_t revLedStartIndex;
    uint8_t revLedEndIndex;
    float revIncrementPerLed;
    // State LED onfig
    bool hasStateLeds;
    uint8_t firstStateLedStartIndex;
    uint8_t firstStateLedEndIndex;
    uint8_t lastStateLedStartIndex;
    uint8_t lastStateLedEndIndex;
    


  public:
    /**
     * @brief Construct a new Sim U Serve Led Hud object
     * with default settings
     * 
     */
    SimUServeLedHud();

    /**
     * @brief Construct a new Sim U Serve Led Hud object
     * with just Rev LEDs 
     * @param numRevLeds The number of rev LEDs
     */
    SimUServeLedHud(uint8_t);

    ~SimUServeLedHud();
    void updateLedState(void);
    
  protected:
    void displayRpmLine(void);
    void flashFlag(CRGB, unsigned long);
    void displayRedline(unsigned long);

  private:
    CRGB indexToColour(uint8_t);
    uint8_t rpmShiftToIndexLimit(double, double);
    void clearLeds(uint8_t, uint8_t);
    void initDefaults(uint8_t);
    uint8_t totalNumberOfLeds(void);
};

#endif
