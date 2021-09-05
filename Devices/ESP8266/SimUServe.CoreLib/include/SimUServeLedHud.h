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

// these come from SimHub.  The first 2 are between 0 and 1 and represent the 2 zones
// for 16 LEDs, we'll go [G,G,G,G,G,G,R,R,R,R,R,B,B,B,B,B]
//                       |--    rpm1   --|--    rpm2   --|
// this fies 0.125 per LED
class SimUServeLedHud {
  protected:

  
  private:
    double rpmShiftLight1;
    double rpmShiftLight2;
    double redLineReached;
    CRGB leds[NUM_LEDS];
    bool flagState;
    bool redlineState;
    bool pitLimitState;
    bool lowFuelState;
    unsigned long previousFlagMillis;
    unsigned long previousRedlineMillis;
    unsigned long previousPitLimitMillis;
    unsigned long previousLowFuelMillis;
    unsigned long flagCycleTime = 500;
    unsigned long redlineCycleTime = 100;

  public:
    SimUServeLedHud();
    ~SimUServeLedHud();
    void updateLedState(void);
    
  protected:
    void displayRpmLine(void);
    void flashFlag(CRGB, unsigned long);
    void displayRedline(unsigned long);

  private:
    CRGB indexToColour(int);
    int rpmShiftToIndexLimit(double, double);
    void clearLeds(int, int);
    void initDefaults(void);
};

#endif
