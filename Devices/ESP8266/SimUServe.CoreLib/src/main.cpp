// used for testing features in the library

#include <Arduino.h>
#include "SimUServeNetworkClient.h"
#include "SimUServeWiFi.h"

SimUServeNetworkClient networkClient = SimUServeNetworkClient();
SimUServeWiFi wifiHelper = SimUServeWiFi();
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

   serviceMode = !wifiHelper.testWifiConnection();
  
   if(serviceMode) 
   {
     Serial.println("Service mode is activated");
     wifiHelper.initServices();
   }

   // put your setup code here, to run once:
   networkClient.udpBeginListening(messageReceivedCallback);
}

void loop() {
  // put your main code here, to run repeatedly:
   if(serviceMode)
   {
     wifiHelper.checkForRequests();
   }
}

