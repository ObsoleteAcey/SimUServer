// used for testing features in the library

#include <Arduino.h>
#include "SimUServeSettingsManager.h"
#include "SimUServeAlphaSegmentDisplay.h"
#include "SimUServeNetworkManager.h"
#include "SimUServeLedHud.h"
#include "SimUServeSevenSegmentDisplay.h"

SimUServeNetworkManager *networkManager;
SimUServeSettingsManager *settingsManager;

bool serviceMode = false;

SimUServeAlphaSegmentDisplay alphaDisplayLeft = SimUServeAlphaSegmentDisplay(0x70);
SimUServeAlphaSegmentDisplay alphaDisplayRight = SimUServeAlphaSegmentDisplay(0x71);
SimUServeLedHud ledHud = SimUServeLedHud();
SimUServeSevenSegmentDisplay sevenSegDisplay = SimUServeSevenSegmentDisplay();

void udpMessageReceivedCallback(AsyncUDPPacket& packet)
{
    Serial.println("Message received");
}

std::function<void(AsyncUDPPacket& packet)> messageReceivedCallback = udpMessageReceivedCallback;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("");
  Serial.println("Startup");

  settingsManager = SimUServeSettingsManager::getSettingsManager();
  settingsManager->setAdvancedSettingsMode(true);
  settingsManager->setConnectedNetworkSsid("DeepSpaceNine");
  settingsManager->setConnectedNetworkSecurityKey("g4l4xy!8o5");
  settingsManager->setConnectedNetworkDeviceIpAddress("192.168.1.50");
  settingsManager->setConnectedNetworkGatewayIpAddress("192.168.1.1");
  settingsManager->setConnectedNetworkNetmask("255.255.255.0");


  networkManager = SimUServeNetworkManager::getNetworkManager();
  networkManager->initConfigServices();
  networkManager->connectToWiFi();

  alphaDisplayLeft.setUpDisplay();
  alphaDisplayRight.setUpDisplay();

  alphaDisplayLeft.writeWord(0, "Test");
  alphaDisplayRight.writeWord(0, "Aiden");

  /*ledHud.initLeds();
  ledHud.updateRpmLedState(0.125, 0, 0);
  ledHud.setFlagColourState(NO_FLAG);*/
  sevenSegDisplay.writeNumber(0,false);
}

void loop() {
  // put your main code here, to run repeatedly:
   //networkManager->checkForRequests();
   ledHud.updateLedState();
}

