#include <Arduino.h>
//#include "SimUServeNetworkClient.h"
#include "SimUServeAlphaSegmentDisplay.h"

SimUServeAlphaSegmentDisplay alphaDisplayLeft = SimUServeAlphaSegmentDisplay(0x70);
SimUServeAlphaSegmentDisplay alphaDisplayRight = SimUServeAlphaSegmentDisplay(0x71);

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("");
  Serial.println("Startup");

  // put your setup code here, to run once:
  alphaDisplayLeft.setUpDisplay();
  alphaDisplayRight.setUpDisplay();


}

void loop() {
  // put your main code here, to run repeatedly:
}