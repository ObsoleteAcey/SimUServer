/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeNetworkManager.h"

#pragma region Constructors / Destructors

SimUServeNetworkManager *SimUServeNetworkManager::_networkManager = nullptr;

SimUServeNetworkManager *SimUServeNetworkManager::getNetworkManager(void)
{
    if (_networkManager == nullptr)
    {
        _networkManager = new SimUServeNetworkManager();
    }

    return _networkManager;
}

SimUServeNetworkManager::SimUServeNetworkManager()
{
    
}

SimUServeNetworkManager::~SimUServeNetworkManager()
{
}