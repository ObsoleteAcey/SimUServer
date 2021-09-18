/*
* Copyright (C) 2021 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/


#ifndef SimUServeNetworkClient_h
#define SimUServeNetworkClient_h

#include "SimUServeNetworkSettings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class SimUServeNetworkClient {
    protected:

    private:
        SimUServeNetworkSettings* _networkSettings;

    public:
        SimUServeNetworkClient();

        /**
         * @brief Construct a new Sim U Serve Network Client object
         * 
         */
        SimUServeNetworkClient(IPAddress, uint16_t);
};


#endif