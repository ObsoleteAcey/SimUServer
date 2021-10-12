// used for testing features in the library

#include <Arduino.h>
#include "SimUServeNetworkManager.h"

SimUServeNetworkManager *_networkManager;

bool serviceMode = false;

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

   _networkManager = SimUServeNetworkManager::getNetworkManager();
  
   _networkManager->initWifiServices();
}

void loop() {
  // put your main code here, to run repeatedly:
   _networkManager->checkForRequests();
}

