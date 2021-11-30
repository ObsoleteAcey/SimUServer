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
        bool _webServiceEnabled;

    public:
        ~SimUServeNetworkManager();

        static SimUServeNetworkManager* getNetworkManager(void);

        void initServiceMode(void);

        void initConfigServices(void);

        void checkForRequests(void);

        /**
         * @brief Attempts to connect to WiFi using the settings saved
         * in settings manager
         * 
         * @return true if connection to WiFi is successful
         * @return false if connection to WiFi is unsuccessful
         */
        bool connectToWiFi(void);
};

#endif