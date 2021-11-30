// used for testing features in the library

#include <Arduino.h>
#include "SimUServeSettingsManager.h"
#include "SimUServeAlphaSegmentDisplay.h"
#include "SimUServeNetworkManager.h"

SimUServeNetworkManager *_networkManager;
SimUServeSettingsManager *_settingsManager;

bool serviceMode = false;

SimUServeAlphaSegmentDisplay alphaDisplayLeft = SimUServeAlphaSegmentDisplay(0x70);
SimUServeAlphaSegmentDisplay alphaDisplayRight = SimUServeAlphaSegmentDisplay(0x71);

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

  _settingsManager = SimUServeSettingsManager::getSettingsManager();
  _settingsManager->setAdvancedSettingsMode(true);
  _settingsManager->setConnectedNetworkSsid("DeepSpaceNine");
  _settingsManager->setConnectedNetworkSecurityKey("g4l4xy!8o5");
  _settingsManager->setConnectedNetworkDeviceIpAddress("192.168.1.50");
  _settingsManager->setConnectedNetworkGatewayIpAddress("192.168.1.1");
  _settingsManager->setConnectedNetworkNetmask("255.255.255.0");


  _networkManager = SimUServeNetworkManager::getNetworkManager();
  _networkManager->initConfigServices();
  _networkManager->connectToWiFi();

  alphaDisplayLeft.setUpDisplay();
  alphaDisplayRight.setUpDisplay();

  alphaDisplayLeft.writeWord(0, "Test");
  alphaDisplayRight.writeWord(0, "Aiden");
}

void loop() {
  // put your main code here, to run repeatedly:
   _networkManager->checkForRequests();
}

