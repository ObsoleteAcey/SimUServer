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
    0x41, // Superscrpt equals, unicode 
}