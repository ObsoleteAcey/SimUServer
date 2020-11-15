/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeSevenSegmentDisplay.h"

// TM1637 encoding - each bit represents a single segment
// 7 Segment display - https://en.wikipedia.org/wiki/Seven-segment_display
// dp | g | f | e | d | c | b | a

// SO to make a 1 for example, we need segments bc to light up, which is 0b110, or 0x06
// as we need a good range, we'll store these as 8 bit unsigned ints

static const uint8_t displayLookUp[] PROGMEM = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
    0x80, // .
    0x01, // Overline, unicode 0x203E
    0x80, // Hyphen, unicode 0x002D
    0x08, // Underline, unicode 0x005F
    0x48, // Equals, unicode 0x003D
    0x41, // Superscrpt equals, unicode 0x207C
    0x63, // Superscript zero, unicode 0x00B0
    0x5C, // Subscript zero
};

SimUServeSevenSegmentDisplay::SimUServeSevenSegmentDisplay()
{
    init();
}

SimUServeSevenSegmentDisplay::SimUServeSevenSegmentDisplay(uint8_t sda, uint8_t scl, uint8_t numberOfDisplays)
{
    init(sda, scl, numberOfDisplays);
}

void SimUServeSevenSegmentDisplay::init(uint8_t sda = ESP_D3, uint8_t scl = ESP_D4, uint8_t numberOfDisplays = 1)
{
    if(numberOfDisplays > 6) 
    {
        numberOfDisplays = 6;
    }
    else if(numberOfDisplays == 0)
    {
        numberOfDisplays = 1;
    }

    _sda = sda;
    _scl = scl;
    
    _displaybuffer = new uint8_t[numberOfDisplays];

    for(uint8_t index = 0; index < numberOfDisplays; index++)
    {
        _displaybuffer[index] = 0;
    }

    pinMode(_sda, OUTPUT);
    pinMode(_scl, OUTPUT);
    digitalWrite(_sda, LOW);
    digitalWrite(_scl, LOW);
}

void SimUServeSevenSegmentDisplay::writeToDisplay(void)
{

}

bool SimUServeSevenSegmentDisplay::writeWordToDisplay(uint8_t wordToWrite)
{
    for(uint8_t bitCount = 0; bitCount < 8; bitCount++)
    {
        // set clock low
        clockLow();

        // write current bit out
        // check with a bitmask for a high or low data signal to send
        wordToWrite & 0x01 ? dataHigh() : dataLow();

        // clock the bit in
        clockHigh();
        // shift the bits over by one
        wordToWrite = wordToWrite >> 1;
    }

    return listenForAck();
}

bool SimUServeSevenSegmentDisplay::listenForAck()
{
    clockLow();
    pinMode(_sda, INPUT);
    clockHigh();

    uint8_t ack = digitalRead(_sda);

    pinMode(_sda, OUTPUT); 
    clockLow();

    return ack;
}

void SimUServeSevenSegmentDisplay::beginTransmission(void)
{
    // clock high, data low
    clockHigh();
    dataLow();
}

void SimUServeSevenSegmentDisplay::clockHigh(void)
{
    digitalWrite(_scl, HIGH);
}

void SimUServeSevenSegmentDisplay::clockLow(void)
{
    digitalWrite(_scl, LOW);
}

void SimUServeSevenSegmentDisplay::dataHigh(void)
{
    digitalWrite(_sda, HIGH);
}

void SimUServeSevenSegmentDisplay::dataLow(void)
{
    digitalWrite(_sda, LOW);
}