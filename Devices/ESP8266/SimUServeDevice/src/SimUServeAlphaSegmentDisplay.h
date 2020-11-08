#ifndef SimUServeAlphaSegmentDisplay_h
#define SimUServeAlphaSegmentDisplay_h

#include "SimUServeCommon.h"

#define DEFAULT_I2C_ADDRESS 0x70
// SETUP stuff
#define SYSTEM_SETUP_REGISTER 0x20
#define SYSTEM_OSCILLATOR_OFF 0x00 // STANDBY MODE
#define SYSTEM_OSCILLATOR_ON 0x01
// Displat stuff
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

class SimUServeAlphaSegmentDisplay {
    private:
        uint16_t* _displaybuffer; //allocate at run time.  Only want the number of elements we are actually writing to
        uint8_t _deviceAddress; // address of the device
        uint8_t _scl, _sda; // the pins to connect the I2C bus to


    public:
        // init with default ports of D2,D1, and 4 displays
        /**
         * @brief  Default constructor
         * @note   Sets the default I2C ports as D2,D1, 4 displays
         * @retval 
         */
        SimUServeAlphaSegmentDisplay();

        SimUServeAlphaSegmentDisplay(uint8_t);

        /**
         * @brief  Constructs using the supplied SDA and SCL port values
         * @note   Defaults to 4 displays and device address of 0x70
         * @retval 
         */
        SimUServeAlphaSegmentDisplay(uint8_t, uint8_t, uint8_t);

        ~SimUServeAlphaSegmentDisplay();

        /**
         * @brief  Inits the display.  Make sure this is called during setup
         * @note   Ensures the display is cleared and ready to rock
         * @retval None
         */
        void init(void);
        
        /**
         * @brief  Writes the current buffer content to the display
         * @note   
         * @retval None
         */
        void writeToDisplay(void);
        
        /**
         * @brief  Write a charcter to the display
         * @note  DOES NOT automatically update the display - this needs to be done manually
         * @param displayNumber - the number of the display to write to (zero based).  Valid values are 0-3.  Any value higher than 
         *                        3 will write to display at index 3
         * @param charToWrite - the character to write.  Must be a valid ASCII character
         * @param includeDecimal - signifies if the decimal should be shown
         * @retval None
         */
        void writeCharacter(uint8_t, uint8_t, bool);

        /**
         * @brief  Writes a string to the displays
         * @note   DOES update the display as needed to show the entire string.  No need to do this manually.
         * @param startDisplay - the display to start writing from
         * @param stringToWrite - the string to write
         * @retval None
         */
        void writeWord(uint8_t, String);

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
};

#endif