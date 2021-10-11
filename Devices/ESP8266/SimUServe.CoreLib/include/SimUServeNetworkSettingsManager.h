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

#define DEFAULT_CONFIG_SERVER_PORT 80
#define DEFAULT_AP_IP "10.0.1.1"
#define DEFAULT_AP_GATEWAY "10.0.1.1"
#define DEFAULT_AP_NETMASK "255.255.255.0"
#define DEFAULT_AP_SSID "SimUServeWiFiHost_"
#define DEFAULT_AP_PASSWORD "SimUServeWiFIPassword"
#define DEFAULT_AP_HOST_NAME "http://simuserve.device"
#define S_UDP_PORT "ServerUdpPort"
#define S_IP_ADDRESS "ServerIpAddress"
#define D_UDP_PORT "DeviceUdpPort"
#define D_CONFIG_SERVER_PORT "DeviceConfigServerPort"
#define CN_SSID "ConnectedNetworkSsid"
#define CN_GATEWAY_IP_ADDRESS "ConnectedNetworkGatewayIpAddress"
#define CN_NETMASK "ConnectedNetworkNetmask"
#define CN_DEVICE_IP "ConnectedNetworkDeviceIp"
#define D_AP_SSID "DeviceApSsid"
#define D_AP_HOST_NAME "DeviceApHostName"
#define D_AP_IP_ADDRESS "DeviceApIpAddress"
#define D_AP_GATEWAY_IP_ADDRESS "DeviceApGatewayIpAddress"
#define D_AP_NETMASK "DeviceApNetmask"

struct NetworkSettings {
    uint16_t ServerUdpPort;
    String ServerIpAddress;

    uint16_t DeviceUdpPort;
    uint16_t DeviceConfigServerPort;
    
    String ConnectedNetworkSsid;
    String ConnectedNetworkSecurityKey;
    String ConnectedNetworkGatewayIpAddress;
    String ConnectedNetworkNetmask;
    String ConnectedNetworkDeviceIp;
    
    
    String DeviceApSsid;
    String DeviceApNetworkSecurityKey;
    String DeviceApHostName;
    String DeviceApIpAddress;
    String DeviceApGatewayIpAddress;
    String DeviceApNetmask;
};

class SimUServeNetworkSettingsManager {
  private:
    bool _isDirty = false;
    IPAddress _apIpAddress;
    IPAddress _apGatewayIpAddress;
    IPAddress _apNetmask;
    IPAddress _cnDeviceIp;
    IPAddress _cnGatewayIpAddress;
    IPAddress _cnNetmask;
    IPAddress _svrIpAddress;
    NetworkSettings _networkSettings;
    static SimUServeNetworkSettingsManager* _settingsManager;
    SimUServeNetworkSettingsManager();

    void loadSettings();

    
  public:
    ~SimUServeNetworkSettingsManager();

    static SimUServeNetworkSettingsManager* getSettingsManager(void);

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

    
    
    #pragma endregion

    #pragma region Related to Device Access Point (Local) server, for setup purposes

    /**
     * @brief Set the SimUServe Device UDP Listen Port
     * The is the port we are listening to for incoming
     * UDP data
     */
    void setDeviceUdpPort(uint16_t const);

    /**
     * @brief Get the SimUServe Device UDP Listen Port
     * 
     * @return uint16_t The server port
     */
    uint16_t getDeviceUdpPort(void) const;

    /**
     * @brief Set the SimUServe Device configuration Port
     * The is the web server port we will be listening on
     */
    void setDeviceConfigServerPort(uint16_t const);

    /**
     * @brief Get the SimUServe Device configuration Port
     * 
     * @return uint16_t The server port
     */
    uint16_t getDeviceConfigServerPort(void) const;

    void setDeviceApSsid(String const&);

    String const& getDeviceApSsid(void) const;

    void setDeviceApNetworkSecurityKey(String const&);

    String const& getDeviceApNetworkSecurityKey(void) const;

    void setDeviceApHostName(String const&);

    String const& getDeviceApHostName(void) const;

    void setDeviceApIpAddress(String const&);

    IPAddress const& getDeviceApIpAddress(void) const;

    void setDeviceApGatewayIpAddress(String const&);

    IPAddress const& getDeviceApGatewayIpAddress(void) const;

    void setDeviceApNetmask(String const&);

    IPAddress const& getDeviceApNetmask(void) const;

    #pragma endregion


    #pragma region Related to connecting to main WiFi access point

    void setConnectedNetworkDeviceIpAddress(String const&);

    IPAddress const& getConnectedNetworkDeviceIpAddress(void) const;

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
    void update(NetworkSettings const, bool);

    void saveSettings(void);
};

#endif
