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

}

SimUServeNetworkClient::SimUServeNetworkClient(IPAddress serverIpAddress, uint16_t serverPort)
{
    
}

#pragma endregion