/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#ifndef SimUServeSevenSegmentDisplay_h
#define SimUServeSevenSegmentDisplay_h

#include "SimUServeCommon.h"

// SETUP stuff
#define SYSTEM_SETUP_REGISTER 0x20
#define SYSTEM_OSCILLATOR_OFF 0x00 // STANDBY MODE
#define SYSTEM_OSCILLATOR_ON 0x01
// Display stuff
#define DISPLAY_SETUP_REGISTER 0x80
#define DISPLAY_ON_COMMAND 0x01
#define DISPLAY_OFF_COMMAND 0x00
#define DISPLAY_BLINK_OFF_COMMAND 0x00
#define DISPLAY_BLINK_TWOHZ_COMMAND 0x01
#define DISPLAY_BLINK_ONEHZ_COMMAND 0x02
#define DISPLAY_BLINK_HALFHZ_COMMAND 0x03

// Brightness settings
#define DISPLAY_BRIGHTNESS_COMMAND 0xEF
#define DISPLAY_BRIGHTNESS_MAX 0x0F

/**
 * @brief  Used for communication with a seven segment display via serial chip - either I2C or some clicked serial data in
 * @note   If using the likes of a TM1637, there will be no slave address to talk to.  It's a pure serial interface and requires
 *          a dedicated data and clock pin.
 * @retval None
 */
class SimUServeSevenSegmentDisplay {
    private:
        uint8_t* _displaybuffer; //allocate at run time.  Only want the number of elements we are actually writing to
        uint8_t _scl, _sda; // the pins to connect the I2C bus to, or the serial data and clock


    public:
        // init with default ports of D2,D1, and 4 displays
        /**
         * @brief  Default constructor
         * @note   Sets the default I2C ports as D2,D1, 4 displays
         * @retval 
         */
        SimUServeSevenSegmentDisplay();

        /**
         * @brief  Constructs using the supplied SDA and SCL port values
         * @note   Defaults to no address and just a single display
         * @param scl - the pin used for the serial clock
         * @param sda - the pin used for the serial data
         * @param numberOfDisplays - the number of displays - 1 to 6
         * @retval 
         */
        SimUServeSevenSegmentDisplay(uint8_t, uint8_t, uint8_t);

        ~SimUServeSevenSegmentDisplay();

        /**
         * @brief  Sets the display up.  Make sure this is called during setup
         * @note   Ensures the display is cleared and ready to rock
         * @retval None
         */
        void setUpDisplay(void);
        
        /**
         * @brief  Writes the current buffer content to the display
         * @note   
         * @retval None
         */
        void writeToDisplay(void);
        
        /**
         * @brief  Write a number to the display
         * @note  DOES automatically update the display.
         * @param numberToDisplay - the number to write to the display
         * @param includeDecimal - signifies if the decimal should be shown
         * @retval None
         */
        void writeNumber(uint8_t, bool);

        /**
         * @brief  Blinks the segement displays
         * @note   
         * @param blinkRate - sets the blink rate for the display.
         *  - DISPLAY_BLINK_OFF_COMMAND - turn blinking off
         *  - DISPLAY_BLINK_TWOHZ_COMMAND - blink at a rate of 2Hz
         *  - DISPLAY_BLINK_ONEHZ_COMMAND - blink at a rate of 1Hz
         *  - DISPLAY_BLINK_HALFHZ_COMMAND - blink at a rate of 1/2Hz
         *  - DEFAULT - DISPLAY_BLINK_TWOHZ_COMMAND
         * @retval None
         */
        void blinkDisplay(uint8_t);

        /**
         * @brief  Writes the current display buffer to the display unit
         * @note   
         * @retval None
         */
        void updateDisplay(void);

        /**
         * @brief  Clears the display buffer and writes it to the display
         * @note   
         * @retval None
         */
        void clearDisplay(void);

        /**
         * @brief  Sets the brightness of the display
         * @note   
         * @param brightness - number between 0 and 15, 0 being dimest and 15 being brightest
         * @retval None
         */
        void setBrightness(uint8_t);
    
    private:
        void init(uint8_t, uint8_t, uint8_t);
};

#endif