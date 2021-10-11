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

class SimUServeNetworkManager {
    private:
        static SimUServeNetworkManager* _networkManager;
        SimUServeNetworkManager();
        bool _isConnected;

    public:
        ~SimUServeNetworkManager();

        static SimUServeNetworkManager* getNetworkManager(void);


};

#endif