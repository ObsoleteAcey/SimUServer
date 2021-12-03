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
#define LED_DEFAULT_TYPE WS2812B
#define LED_DEFAULT_DATA_PIN ESP_D5
#define LED_DEFAULT_COLOUR_ORDER GRB
#define LED_DEFAULT_COLOUR_CORRECTION TypicalLEDStrip
#define LED_DEFAULT_BRIGHTNESS 50
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

enum FlagColourState {
  NO_FLAG = 1,
  YELLOW_FLAG = 2,
  BLUE_FLAG = 3,
  PIT_LIMIT = 4
};


/**
 * @brief Class for driving an LED hud.  Can be used with just a rev LED strip, or rev and
 * status LEDs
 * 
 */
class SimUServeLedHud {
  private:
    double _rpmShiftLight1;
    double _rpmShiftLight2;
    bool _redLineReached;
    FlagColourState _flagColourState;
    bool _flagState;
    bool _redlineState;
    bool _pitLimitState;
    bool _lowFuelState;
    
    // timer state attributes
    uint32_t _currentMillis;
    uint32_t _previousFlagMillis;
    uint32_t _previousRedlineMillis;
    uint32_t _previousPitLimitMillis;
    uint32_t _previousLowFuelMillis;
    uint16_t _flagCycleTime;
    uint16_t _redlineCycleTime;
    /*---------- LED config settings -------------*/
    // array of LEDs
    CRGB *_leds;
    // Rev LED config
    uint8_t _numberOfRevLeds;
    uint8_t _revLedStartIndex;
    uint8_t _revLedEndIndex;
    float _revIncrementPerLed;
    // State LED onfig
    bool _hasStateLeds;
    uint8_t _firstStateLedStartIndex;
    uint8_t _firstStateLedEndIndex;
    uint8_t _lastStateLedStartIndex;
    uint8_t _lastStateLedEndIndex;

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

    /**
     * @brief Updates the brightness of the LEDs
     * @param brightness brightness to change to
     */
    void updateLedBrightness(uint8_t);


    /**
     * @brief Sets up the LEDs based on the current config
     * This should be the last thing called.  Uses LED_DEFAULT_TYPE for LED Type,
     * LED_DEFAULT_DATA_PIN for the data pin,
     * LED_DEFAULT_COLOUR_ORDER for the colour order,
     * LED_DEFAULT_COLOUR_CORRECTION for the colour correction, 
     * and LED_DEFAULT_BRIGHTNESS for the brightness.
     * @return SimUServeLedHud* 
     */
    SimUServeLedHud* initLeds(void);

    /**
     * @brief Sets up the LEDs based on the current config
     * 
     * @tparam SPIChipsets The chipset being used.  Use LED_DEFAULT_TYPE for default
     * @tparam uint8_t The port pin to use for data.  Use LED_DEFAULT_DATA_PIN for default
     * @tparam EOrder The order of the colours in the LED strip.  Use LED_DEFAULT_COLOUR_ORDER for default
     * @param colourCorrection Colour correction to be applied to the LED strip
     * @param brightness brightness of the LED strip
     * @return SimUServeLedHud* 
     * 
     */
    template<ESPIChipsets,  uint8_t, EOrder> SimUServeLedHud* initLeds(LEDColorCorrection, uint8_t);

    /**
     * @brief 
     * @param rpmShiftLight1 description
     * @param rpmShiftLight2 
     * @param redLineReached true of the redline has been reached
     */
    void updateRpmLedState(double, double, bool);

    /**
     * @brief Set the Flag Colour State attribute
     * 
     */
    void setFlagColourState(FlagColourState);
   
  private:
    void displayRpmLine(void);
    void flashFlag(CRGB);
    void displayRedline();
    CRGB indexToColour(uint8_t);
    CRGB flagStateToColour(FlagColourState);
    uint8_t rpmShiftToIndexLimit(double, double);
    void clearLeds(uint8_t, uint8_t);
    void initDefaults(uint8_t);
    uint8_t totalNumberOfLeds(void);
};

#endif
