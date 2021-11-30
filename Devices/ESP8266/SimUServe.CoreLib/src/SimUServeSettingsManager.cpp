/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeSettingsManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#pragma region Constructors / Destructors

SimUServeSettingsManager *SimUServeSettingsManager::_settingsManager = nullptr;

SimUServeSettingsManager *SimUServeSettingsManager::getSettingsManager(void)
{
    if (_settingsManager == nullptr)
    {
        _settingsManager = new SimUServeSettingsManager();
    }

    return _settingsManager;
}

SimUServeSettingsManager::SimUServeSettingsManager()
{
    _networkSettings = NetworkSettings();
    this->loadSettings();
}

SimUServeSettingsManager::~SimUServeSettingsManager()
{
}

void SimUServeSettingsManager::loadSettings()
{
    Serial.println("SimUServeSettingsManager::loadSettings");
    if(!LittleFS.begin())
    {
        Serial.println("Could not mount LittleFS");
        return;
    }

    File file = LittleFS.open("/settings.json", "r");

    String jsonSettings = file.readString();
    file.close();

    StaticJsonDocument<768> doc;

    Serial.println("Deserializing JSON settings");
    deserializeJson(doc, jsonSettings);

    // set the PC server settings
    this->setServerIpAddress(doc[S_IP_ADDRESS]);
    this->setServerUdpPort(doc[S_UDP_PORT].as<uint16_t>());

    // get the network stuff from the WiFi class
    this->setConnectedNetworkSecurityKey(WiFi.psk());
    // but the remainder from the settings
    this->setConnectedNetworkSsid(doc[CN_SSID]);
    this->setConnectedNetworkGatewayIpAddress(doc[CN_GATEWAY_IP_ADDRESS]);
    this->setConnectedNetworkDeviceIpAddress(doc[CN_DEVICE_IP]);
    this->setConnectedNetworkNetmask(doc[CN_NETMASK]);

    // access point related config
    this->setDeviceApHostName(doc[D_AP_HOST_NAME] | DEFAULT_AP_HOST_NAME);
    this->setDeviceApIpAddress(doc[D_AP_IP_ADDRESS] | DEFAULT_AP_IP);
    this->setDeviceApSsid(doc[D_AP_SSID] | DEFAULT_AP_SSID);
    this->setDeviceApNetworkSecurityKey(doc[D_AP_NETWORK_KEY] | DEFAULT_AP_NETWORK_KEY);
    
    this->setDeviceConfigServerPort(doc[D_CONFIG_SERVER_PORT] | DEFAULT_CONFIG_SERVER_PORT);
    Serial.println("Settings Deserialized successfully");
}

#pragma endregion

#pragma region Related to remote(PC) SimUServe server

void SimUServeSettingsManager::setServerUdpPort(uint16_t const port)
{
    if (this->_networkSettings.ServerUdpPort != port)
    {
        this->_isDirty = true;
        this->_networkSettings.ServerUdpPort = port;
    }
}

uint16_t SimUServeSettingsManager::getServerUdpPort(void) const
{
    return this->_networkSettings.ServerUdpPort;
}

void SimUServeSettingsManager::setServerIpAddress(String const &ipAddress)
{
    if (this->_networkSettings.ServerIpAddress != ipAddress)
    {
        this->_isDirty = true;
        this->_networkSettings.ServerIpAddress = String(ipAddress);
        if (!this->_svrIpAddress)
        {
            this->_svrIpAddress = IPAddress();
        }
        this->_svrIpAddress.fromString(ipAddress);
    }
}

IPAddress const &SimUServeSettingsManager::getServerIpAddress(void) const
{
    return this->_svrIpAddress;
}

#pragma endregion

#pragma region Related to Device Access Point(Local) server, for setup purposes

void SimUServeSettingsManager::setDeviceUdpPort(uint16_t const port)
{
    if (this->_networkSettings.DeviceUdpPort != port)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceUdpPort = port;
    }
}

uint16_t SimUServeSettingsManager::getDeviceUdpPort(void) const
{
    return this->_networkSettings.DeviceUdpPort;
}

void SimUServeSettingsManager::setDeviceConfigServerPort(uint16_t const port)
{
    if (this->_networkSettings.DeviceConfigServerPort != port)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceConfigServerPort = port;
    }
}

uint16_t SimUServeSettingsManager::getDeviceConfigServerPort(void) const
{
    return this->_networkSettings.DeviceConfigServerPort;
}

void SimUServeSettingsManager::setDeviceApSsid(String const &deviceApSsid)
{
    if (this->_networkSettings.DeviceApSsid != deviceApSsid)
    {
        _isDirty = true;
        this->_networkSettings.DeviceApSsid = String(deviceApSsid);
    }
}

String const &SimUServeSettingsManager::getDeviceApSsid(void) const
{
    return this->_networkSettings.DeviceApSsid;
}

void SimUServeSettingsManager::setDeviceApNetworkSecurityKey(String const &apSecurityKey)
{
    if (this->_networkSettings.DeviceApNetworkSecurityKey != apSecurityKey)
    {
        _isDirty = true;
        this->_networkSettings.DeviceApNetworkSecurityKey = String(apSecurityKey);
    }
}

String const &SimUServeSettingsManager::getDeviceApNetworkSecurityKey(void) const
{
    return this->_networkSettings.DeviceApNetworkSecurityKey;
}

void SimUServeSettingsManager::setDeviceApHostName(String const &deviceHostName)
{
    if (this->_networkSettings.DeviceApHostName != deviceHostName)
    {
        _isDirty = true;
        this->_networkSettings.DeviceApHostName = String(deviceHostName);
    }
}

String const &SimUServeSettingsManager::getDeviceApHostName(void) const
{
    return this->_networkSettings.DeviceApHostName;
}

void SimUServeSettingsManager::setDeviceApIpAddress(String const &deviceApIp)
{
    if (this->_networkSettings.DeviceApIpAddress != deviceApIp)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceApIpAddress = String(deviceApIp);
        if (!this->_apIpAddress)
        {
            this->_apIpAddress = IPAddress();
        }
        this->_apIpAddress.fromString(deviceApIp);
    }
}

IPAddress const &SimUServeSettingsManager::getDeviceApIpAddress(void) const
{
    return this->_apIpAddress;
}

void SimUServeSettingsManager::setDeviceApGatewayIpAddress(String const &deviceApGatewayIp)
{
    if (this->_networkSettings.DeviceApGatewayIpAddress != deviceApGatewayIp)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceApGatewayIpAddress = String(deviceApGatewayIp);
        if (!this->_apGatewayIpAddress)
        {
            this->_apGatewayIpAddress = IPAddress();
        }
        this->_apGatewayIpAddress.fromString(deviceApGatewayIp);
    }
}

IPAddress const &SimUServeSettingsManager::getDeviceApGatewayIpAddress(void) const
{
    return this->_apGatewayIpAddress;
}

void SimUServeSettingsManager::setDeviceApNetmask(String const &apNetmaskIp)
{
    if (this->_networkSettings.DeviceApNetmask != apNetmaskIp)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceApNetmask = String(apNetmaskIp);
        if (!this->_apNetmask)
        {
            this->_apNetmask = IPAddress();
        }
        this->_apNetmask.fromString(apNetmaskIp);
    }
}

IPAddress const &SimUServeSettingsManager::getDeviceApNetmask(void) const
{
    return this->_apNetmask;
}

#pragma endregion

#pragma region Related to connecting to main WiFi access point

void SimUServeSettingsManager::setConnectedNetworkDeviceIpAddress(String const &deviceIp)
{
    if (this->_networkSettings.ConnectedNetworkDeviceIp != deviceIp)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkDeviceIp = String(deviceIp);
        if (!this->_cnDeviceIp)
        {
            this->_cnDeviceIp = IPAddress();
        }
        this->_cnDeviceIp.fromString(deviceIp);
    }
}

IPAddress const &SimUServeSettingsManager::getConnectedNetworkDeviceIpAddress(void) const
{
    return this->_cnDeviceIp;
}

void SimUServeSettingsManager::setConnectedNetworkGatewayIpAddress(String const &gatewayIp)
{
    if (this->_networkSettings.ConnectedNetworkGatewayIpAddress != gatewayIp)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkGatewayIpAddress = String(gatewayIp);
        if (!this->_cnGatewayIpAddress)
        {
            this->_cnGatewayIpAddress = IPAddress();
        }
        this->_cnGatewayIpAddress.fromString(gatewayIp);
    }
};

IPAddress const &SimUServeSettingsManager::getConnectedNetworkGatewayIpAddress(void) const
{
    return this->_cnGatewayIpAddress;
};

void SimUServeSettingsManager::setConnectedNetworkNetmask(String const &netmask)
{
    if (this->_networkSettings.ConnectedNetworkNetmask != netmask)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkNetmask = String(netmask);
        if (!this->_cnNetmask)
        {
            this->_cnNetmask = IPAddress();
        }
        this->_cnNetmask.fromString(netmask);
    }
};

IPAddress const &SimUServeSettingsManager::getConnectedNetworkNetmask(void) const
{
    return this->_cnNetmask;
}

void SimUServeSettingsManager::setConnectedNetworkSsid(String const &connectedSsid)
{
    if (this->_networkSettings.ConnectedNetworkSsid != connectedSsid)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkSsid = String(connectedSsid);
    }
}

String const &SimUServeSettingsManager::getConnectedNetworkSsid(void) const
{
    return this->_networkSettings.ConnectedNetworkSsid;
}

void SimUServeSettingsManager::setConnectedNetworkSecurityKey(String const &securityKey)
{
    if (this->_networkSettings.ConnectedNetworkSecurityKey != securityKey)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkSecurityKey = String(securityKey);
    }
}

String const &SimUServeSettingsManager::getConnectedNetworkSecurityKey(void) const
{
    return this->_networkSettings.ConnectedNetworkSecurityKey;
}

void SimUServeSettingsManager::setAdvancedSettingsMode(bool advancedMode)
{
    this->_networkSettings.AdvancedSettingsMode = advancedMode;
}

bool SimUServeSettingsManager::getAdvancedSettingsMode(void)
{
    return this->_networkSettings.AdvancedSettingsMode;
}

#pragma endregion

void SimUServeSettingsManager::setClean(void)
{
    this->_isDirty = false;
}

void SimUServeSettingsManager::update(NetworkSettings const fromSettings, bool autoPersist)
{
    // TODO - some kind fo detection for default vs loaded vs dirty
    // set up server settings
    this->setServerIpAddress(fromSettings.ServerIpAddress);
    this->setServerUdpPort(fromSettings.ServerUdpPort);

    // set up AP settings
    this->setDeviceApSsid(fromSettings.DeviceApSsid);
    this->setDeviceApSsid(fromSettings.DeviceApSsid);
    this->setDeviceApIpAddress(fromSettings.DeviceApIpAddress);
    this->setDeviceApNetworkSecurityKey(fromSettings.DeviceApNetworkSecurityKey);
    this->setDeviceApGatewayIpAddress(fromSettings.DeviceApGatewayIpAddress);
    this->setDeviceApHostName(fromSettings.DeviceApHostName);

    if (autoPersist)
    {
        this->saveSettings();
    }
}

void SimUServeSettingsManager::saveSettings(void)
{
    if(!this->_isDirty)
    {
        Serial.println("Settings not dirty - not saving");
        return;
    }

    if(!LittleFS.begin())
    {
        Serial.println("Counld not mount LittleFS to save settings");
        return;
    }

    // TODO - handle saving this to EEPROM or FS
    // File file = LittleFS.open("/config.json", "w");
    
    

}