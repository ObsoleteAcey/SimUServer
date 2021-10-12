/*
* Copyright (C) 2021 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/


#ifndef SimUServeNetworkManager_h
#define SimUServeNetworkManager_h

#include "SimUServeWifi.h"
#include "SimUServeNetworkClient.h"

class SimUServeNetworkManager {
    private:
        static SimUServeNetworkManager* _networkManager;
        SimUServeWiFi* _wifi;
        SimUServeNetworkClient* _networkClient;

        SimUServeNetworkManager();
        bool _isConnected;
        bool _serviceMode;

    public:
        ~SimUServeNetworkManager();

        static SimUServeNetworkManager* getNetworkManager(void);

        void initWifiServices(void);

        void checkForRequests(void);


};

#endif