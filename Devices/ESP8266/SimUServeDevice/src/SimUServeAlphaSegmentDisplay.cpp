#include "SimUServeAlphaSegmentDisplay.h"
#include <Wire.h>


// HT16K33 encoding - each bit represents a single segment
// 14 Segment display - https://en.wikipedia.org/wiki/Fourteen-segment_display
// dp | m | l | k | j | i | h | g2  | g1 | f | e | d | c | b | a

// SO to make a C for example, we need segments afed to light up, which is 0b111001, or 0x39
// as we need a good range, we'll store these as 16 bit unsigned ints

static const uint16_t displayLookUp[] PROGMEM = {
    0x0000, //
    0x4006, // !
    0x0202, // "
    0x12CE, // #
    0x12ED, // $
    0x0C24, // %
    0x235D, // &
    0x0400, // '
    0x2400, // (
    0x0900, // )
    0x3FC0, // *
    0x12C0, // +
    0x0800, // ,
    0x00C0, // -
    0x4000, // .
    0x0C00, // /
    0x0C3F, // 0
    0x0406, // 1
    0x00DB, // 2
    0x008F, // 3
    0x00E6, // 4
    0x00ED, // 5
    0x00FD, // 6
    0x1401, // 7
    0x00FF, // 8
    0x00E7, // 9
    0x1200, // :
    0x0A00, // ;
    0x2440, // <
    0x00C8, // =
    0x0980, // >
    0x1083, // ?
    0x02BB, // @
    0x00F7, // A
    0x128F, // B
    0x0039, // C
    0x120F, // D
    0x00F9, // E
    0x00F1, // F
    0x00BD, // G
    0x00F6, // H
    0x1209, // I
    0x001E, // J
    0x2470, // K
    0x0038, // L
    0x0536, // M
    0x2136, // N
    0x003F, // O
    0x00F3, // P
    0x203F, // Q
    0x20F3, // R
    0x018D, // S
    0x1201, // T
    0x003E, // U
    0x0C30, // V
    0x2836, // W
    0x2D00, // X
    0x1500, // Y
    0x0C09, // Z
    0x0039, // [
    0x2100, // backslash
    0x000F, // ]
    0x2800, // ^
    0x0008, // _
    0x0100, // `
    0x1058, // a 
    0x2078, // b 
    0x00D8, // c 
    0x088E, // d 
    0x0858, // e 
    0x14C0, // f 
    0x048E, // g 
    0x1070, // h 
    0x1000, // i 
    0x0A10, // j 
    0x3600, // k 
    0x0030, // l 
    0x10D4, // m 
    0x1050, // n 
    0x00DC, // o 
    0x0170, // p 
    0x0486, // q 
    0x0050, // r 
    0x2088, // s 
    0x0078, // t 
    0x001C, // u 
    0x0810, // v 
    0x2814, // w 
    0x2D00, // x 
    0x028E, // y 
    0x0848, // z 
    0x0949, // { 
    0x1200, // | 
    0x2489, // } 
    0x0CC0 // ~ 
};

SimUServeAlphaSegmentDisplay::SimUServeAlphaSegmentDisplay()
{
    _deviceAddress = DEFAULT_I2C_ADDRESS;
    _sda = ESP_D2;
    _scl = ESP_D1;
    _displaybuffer = new uint16_t[4] { 0,0,0,0 };
}

SimUServeAlphaSegmentDisplay::SimUServeAlphaSegmentDisplay(uint8_t deviceAddress)
{
    _deviceAddress = deviceAddress;
    _sda = ESP_D2;
    _scl = ESP_D1;
    _displaybuffer = new uint16_t[4] { 0,0,0,0 };
}

SimUServeAlphaSegmentDisplay::SimUServeAlphaSegmentDisplay(uint8_t sda, uint8_t scl, uint8_t deviceAddress = DEFAULT_I2C_ADDRESS)
{
    _deviceAddress = deviceAddress;
    _sda = sda;
    _scl = scl;
    _displaybuffer = new uint16_t[4] { 0,0,0,0 };
}

SimUServeAlphaSegmentDisplay::~SimUServeAlphaSegmentDisplay()
{
    delete[] _displaybuffer;
}

void SimUServeAlphaSegmentDisplay::init(void)
{
    // we need to init the chip and get ready for display
    Serial.println("Starting I2C on SDA:" + String(_sda) + " SCL:" + String(_scl) + " Address:" + String(_deviceAddress));
    Wire.begin(_sda, _scl);
    Wire.beginTransmission(_deviceAddress);
    Wire.write(SYSTEM_SETUP_REGISTER | SYSTEM_OSCILLATOR_ON);
    Wire.endTransmission();

    clearDisplay();

    // now turn the display on
    Wire.beginTransmission(_deviceAddress);
    Wire.write(DISPLAY_SETUP_REGISTER | DISPLAY_ON_COMMAND);
    Wire.endTransmission();
}

void SimUServeAlphaSegmentDisplay::writeToDisplay(void)
{
    Wire.beginTransmission(_deviceAddress);
    Wire.write((uint8_t)0x00);

    for (uint8_t index = 0; index < 4; index++) {
        Wire.write(_displaybuffer[index] & 0xFF); // take the first 8 bits
        Wire.write(_displaybuffer[index] >> 8);  // take the last 8 bits
    } 

    Wire.endTransmission();    
}

void SimUServeAlphaSegmentDisplay::writeCharacter(uint8_t displayNumber, uint8_t charToWrite, bool includeDecimal = false)
{
    if (displayNumber > 3)
    {
        displayNumber = 3;
    }

    uint8_t address = (charToWrite - ASCII_START_CHAR);
    _displaybuffer[displayNumber] = pgm_read_word(displayLookUp + address); // subtract the offset as we don't use the first 30 odd ASCII chars
    Serial.println("Writing: " + String(_displaybuffer[displayNumber]));
    if(includeDecimal)
    {
        _displaybuffer[displayNumber] = _displaybuffer[displayNumber] | displayLookUp[14];
    }
}

void SimUServeAlphaSegmentDisplay::writeWord(uint8_t startDisplay, String stringToWrite)
{
    bool containsDecimal = stringToWrite.indexOf('.') > 0;
    int displayNumber = 0;
    // cycle through the string, writing to the display. 
    // TODO: If the string is longer, we should scroll it along the whole display.
    for(uint8_t index = 0; index < stringToWrite.length(); index++)
    {
        // check if there's a decimal - no need to boundry check as String[] operator seems to handle out of bounds
        bool writeDecimal = stringToWrite[index + 1] == '.';
        writeCharacter(displayNumber, stringToWrite[index], writeDecimal);
        if(writeDecimal)
        {
            index++; // skip the decimal on next loop
        }
        displayNumber++;
    }

    writeToDisplay();

}

void SimUServeAlphaSegmentDisplay::blinkDisplay(uint8_t blinkRate)
{
    if (blinkRate > DISPLAY_BLINK_HALFHZ_COMMAND)
    {
        blinkRate = DISPLAY_BLINK_HALFHZ_COMMAND;
    }
    
    Wire.beginTransmission(_deviceAddress);
    Wire.write(DISPLAY_SETUP_REGISTER | (blinkRate << 1));
    Wire.endTransmission();
}

void SimUServeAlphaSegmentDisplay::clearDisplay(void)
{
    // set the buffer to zero
    for(uint8_t index = 0; index < 4; index++)
    {
        _displaybuffer[index] = 0x00;
    }

    writeToDisplay();
}

void SimUServeAlphaSegmentDisplay::setBrightness(uint8_t brightness)
{
    if(brightness > DISPLAY_BRIGHTNESS_MAX)
    {
        brightness = DISPLAY_BRIGHTNESS_MAX;
    }

    Wire.beginTransmission(_deviceAddress);
    Wire.write(DISPLAY_BRIGHTNESS_COMMAND & brightness);
    Wire.endTransmission();
}