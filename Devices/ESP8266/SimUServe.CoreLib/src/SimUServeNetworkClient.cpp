/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeNetworkClient.h"

#pragma region Constructors / Destructor

SimUServeNetworkClient::SimUServeNetworkClient()
{
    this->_networkSettings = SimUServeNetworkSettingsManager::getSettingsManager();
    this->_udp = nullptr;
}

SimUServeNetworkClient::SimUServeNetworkClient(String serverIpAddress, uint16_t serverPort)
{
    this->_networkSettings = SimUServeNetworkSettingsManager::getSettingsManager();
    this->_networkSettings->setServerIpAddress(serverIpAddress);
    this->_networkSettings->setServerUdpPort(serverPort);
    this->_networkSettings->saveSettings();
    this->_udp = nullptr;
}

SimUServeNetworkClient::~SimUServeNetworkClient()
{
    delete _udp;
}

#pragma endregion

bool SimUServeNetworkClient::udpBeginListening(AuPacketHandlerFunction callback)
{
    Serial.println("SimUServeNetworkClient::udpBeginListening");
    if (this->_udp == nullptr)
    {
        this->_udp = new AsyncUDP();
    }

    // prevent multiple connection attempts
    if (this->_udp->connected())
    {
        Serial.println("Already connected to UDP");
        return true;
    }

    Serial.println("Beginning listen on " + this->_networkSettings->getServerIpAddress().toString() + " port " + String(this->_networkSettings->getDeviceUdpPort()));
    bool isListening = this->_udp->listen(this->_networkSettings->getServerIpAddress(), this->_networkSettings->getDeviceUdpPort());

    if (isListening)
    {
        Serial.println("Now listening - setting callback");
        this->_udp->onPacket(callback);
    }

    return isListening;
}

bool SimUServeNetworkClient::sendMessage(String const &messageToSend)
{
    if (this->_udp == nullptr)
    {
        this->_udp = new AsyncUDP();
    }

    AsyncUDPMessage message = AsyncUDPMessage(messageToSend.length());
    message.print(messageToSend);

    this->_udp->sendTo(message, this->_networkSettings->getServerIpAddress(), this->_networkSettings->getServerUdpPort());

    return true;
}
