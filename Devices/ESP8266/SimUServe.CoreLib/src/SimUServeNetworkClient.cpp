/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeNetworkClient.h"

#pragma region Constructors/Destructor

SimUServeNetworkClient::SimUServeNetworkClient()
{
    this->_networkSettings = SimUServeNetworkSettings::getSettings();
    this->_udp = nullptr;
}

SimUServeNetworkClient::SimUServeNetworkClient(String serverIpAddress, uint16_t serverPort)
{
    this->_networkSettings = SimUServeNetworkSettings::getSettings();
    this->_networkSettings->setServerIpAddress(serverIpAddress);
    this->_networkSettings->setServerUdpPort(serverPort);
    this->_networkSettings->saveSettings();
    this->_udp = nullptr;
}

SimUServeNetworkClient::~SimUServeNetworkClient()
{
    delete _udp;
}

void SimUServeNetworkClient::udpBeginListening(AuPacketHandlerFunction callback)
{
    if(this->_udp == nullptr)
    {
        _udp = new AsyncUDP();
    }

    if(_udp->connected())
    {
        return;
    }

    if(_udp->connect(this->_networkSettings->getServerIpAddress(), this->_networkSettings->getServerUdpPort()))
    {
        
    }
}

#pragma endregion