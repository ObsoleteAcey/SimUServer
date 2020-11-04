#include <FastLED.h>

#define NUM_LEDS 22
#define NUM_LEDS_REV 16
#define DATA_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 50
#define FIRST_STATE_LED_START_INDEX 0
#define FIRST_STATE_LED_END_INDEX 2
#define LAST_STATE_LED_START_INDEX 19
#define LAST_STATE_LED_END_INDEX 21
#define REV_DISPLAY_START_INDEX 3
#define REV_DISPLAY_END_INDEX 18

// these come from SimHub.  The first 2 are between 0 and 1 and represent the 2 zones
// for 16 LEDs, we'll go [G,G,G,G,G,G,R,R,R,R,R,B,B,B,B,B]
//                       |-- rpm1      --|--    rpm2  --|
// this fies 0.125 per LED
double rpmShiftLight1 = 1, rpmShiftLight2 = 0.9, redLineReached = 1;

CRGB leds[NUM_LEDS];

bool flagState = LOW;
bool redlineState = LOW;
bool pitLimitState = LOW;
bool lowFuelState = LOW;

unsigned long previousFlagMillis = 0;
unsigned long previousRedlineMillis = 0;
unsigned long previousPitLimitMillis = 0;
unsigned long previousLowFuelMillis = 0;

long flagCycleTime = 500;
long redlineCycleTime = 100;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  delay(3000); // initial delay of a few seconds is recommended
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);// global brightness
}

void flashFlag(CRGB colour, unsigned long currentMillis)
{
  Serial.println("Current millis: " + String(currentMillis) + " previousFlagMillis: " + String(previousFlagMillis));
  if(currentMillis - previousFlagMillis < flagCycleTime)
  {
    return;
  }
  Serial.println("Changing flag state: " + String(flagState));
  for(int index = FIRST_STATE_LED_START_INDEX; index <= LAST_STATE_LED_END_INDEX; index++)
  {
    leds[index] = flagState ? colour : CRGB::Black;
    if(index == FIRST_STATE_LED_END_INDEX)
    {
      index = LAST_STATE_LED_START_INDEX - 1;
    }
  }

  flagState = !flagState;
  previousFlagMillis = currentMillis;
}

CRGB indexToColour(int index) {
  if(index >= 3 && index < 9)
  {
    return CRGB::Green;
  }
  else if (index >= 9 && index < 14)
  {
    return CRGB::Red;
  }

  return CRGB::Blue;
}

int rpmShiftToIndexLimit(double rpm1, double rpm2)
{
  int index = (int)((rpm1+rpm2)/0.125);
  return index;
}

void displayRedline(unsigned long currentMillis)
{
  if(currentMillis - previousRedlineMillis < redlineCycleTime)
  {
    return;
  }
  for (int i = REV_DISPLAY_START_INDEX; i <= REV_DISPLAY_END_INDEX; i++) {
    leds[i] = redlineState ? CRGB::Blue : CRGB::Black;
  }

  previousRedlineMillis = currentMillis;
  redlineState = !redlineState;
}

// switches off all LEDs
void clearLeds(int startIndex, int endIndex) {
  for (int i = startIndex; i <= endIndex; i++) {
    leds[i] = CRGB::Black;
  }
}

void displayRpmLine()
{

  int indexMax = rpmShiftToIndexLimit(rpmShiftLight1, rpmShiftLight2);

  if(indexMax == 0)
  {
    clearLeds(REV_DISPLAY_START_INDEX, REV_DISPLAY_END_INDEX);
    return;
  }

  indexMax+=3;

  for(int index = 3; index < indexMax; index++)
  {
    leds[index] = indexToColour(index);
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  
  if(redLineReached) 
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
