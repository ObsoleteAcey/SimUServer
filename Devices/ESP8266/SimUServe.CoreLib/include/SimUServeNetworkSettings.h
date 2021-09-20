/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#ifndef SimUServeNetworkSettings_h
#define SimUServeNetworkSettings_h

#include <ESP8266WiFi.h>
#include <stdint.h>
#include <String>

#define DEFAULT_SERVER_PORT 80

class SimUServeNetworkSettings {
  protected:
    uint16_t _serverUdpPort;
    String _serverIpAddress;

    uint16_t _deviceUdpPort;
    uint16_t _deviceConfigServerPort;
    
    String _connectedNetworkSsid;
    String _connectedNetworkSecurityKey;
    String _connectedNetworkGatewayIpAddress;
    String _connectedNetworkNetmask;
    
    
    String _deviceApSsid;
    String _deviceApNetworkSecurityKey;
    String _deviceApHostName;
    String _deviceApIpAddress;
    String _deviceApGatewayIpAddress;
    String _deviceApNetmask;


  private:
    bool _isDirty = false;
    IPAddress _apIpAddress;
    IPAddress _apGatewayIpAddress;
    IPAddress _apNetmask;
    IPAddress _cnGatewayIpAddress;
    IPAddress _cnNetmask;
    IPAddress _svrIpAddress;
    static SimUServeNetworkSettings* _settings;
    
  public:
    ~SimUServeNetworkSettings();

    static SimUServeNetworkSettings* getSettings(void);

    #pragma region Related to remote (PC) SimUServe server

    /**
     * @brief Set the SimUServe Server Port
     * The is, the server we are connecting to, usually hosted
     * on a PC
     */
    void setServerUdpPort(uint16_t const);

    /**
     * @brief Get the SimUServe Server Port
     * 
     * @return int The server port
     */
    uint16_t getServerUdpPort(void) const;

    /**
     * @brief Set the SimUServe Server Ip Address
     * The is, the server we are connecting to, usually hosted
     * on a PC
     */
    void setServerIpAddress(String const&);

    /**
     * @brief Get the SimUServe Server Ip Address
     * 
     * @return An IPAddress object containing information about the 
     * SimUServe server IP Address
     */
    IPAddress const& getServerIpAddress(void) const;

    /**
     * @brief Set the SimUServe Server Port
     * The is, the server we are connecting to, usually hosted
     * on a PC
     */
    void setDeviceUdpPort(uint16_t const);

    /**
     * @brief Get the SimUServe Server Port
     * 
     * @return int The server port
     */
    uint16_t getDeviceUdpPort(void) const;

    /**
     * @brief Set the SimUServe Server Port
     * The is, the server we are connecting to, usually hosted
     * on a PC
     */
    void setDeviceConfigServerPort(uint16_t const);

    /**
     * @brief Get the SimUServe Server Port
     * 
     * @return int The server port
     */
    uint16_t getDeviceConfigServerPort(void) const;
    
    #pragma endregion

    #pragma region Related to Device Access Point (Local) server, for setup purposes

    void setDeviceApSsid(String const&);

    String const& getDeviceApSsid(void) const;

    void setDeviceApNetworkSecurityKey(String const&);

    String const& getDeviceApNetworkSecurityKey(void) const;

    void setDeviceApHostName(String const&);

    String const& getDeviceApHostName(void) const;

    void setDeviceApIpAddress(String const&);

    IPAddress const& getDeviceApIpAddress(void) const;

    void setDeviceApGatwayIpAddress(String const&);

    IPAddress const& getDeviceApGatwayIpAddress(void) const;

    void setDeviceApNetmask(String const&);

    IPAddress const& getDeviceApNetmask(void) const;

    #pragma endregion


    #pragma region Related to connecting to main WiFi access point

    void setConnectedNetworkGatewayIpAddress(String const&);

    IPAddress const& getConnectedNetworkGatewayIpAddress(void) const;

    void setConnectedNetworkNetmask(String const&);

    IPAddress const& getConnectedNetworkNetmask(void) const;
    
    void setConnectedNetworkSsid(String const&);

    String const& getConnectedNetworkSsid(void) const;

    void setConnectedNetworkSecurityKey(String const&);

    String const& getConnectedNetworkSecurityKey(void) const;

    #pragma endregion
    
    /*
     * Resets the dirty flag
    */
    void setClean(void);
    void update(SimUServeNetworkSettings const&, bool);

    void saveSettings(void);
};

#endif
