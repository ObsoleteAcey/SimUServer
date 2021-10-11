/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeNetworkSettingsManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#pragma region Constructors / Destructors

SimUServeNetworkSettingsManager *SimUServeNetworkSettingsManager::_settingsManager = nullptr;

SimUServeNetworkSettingsManager *SimUServeNetworkSettingsManager::getSettingsManager(void)
{
    if (_settingsManager == nullptr)
    {
        _settingsManager = new SimUServeNetworkSettingsManager();
    }

    return _settingsManager;
}

SimUServeNetworkSettingsManager::SimUServeNetworkSettingsManager()
{
    _networkSettings = NetworkSettings();
    this->loadSettings();
}

SimUServeNetworkSettingsManager::~SimUServeNetworkSettingsManager()
{
}

void SimUServeNetworkSettingsManager::loadSettings()
{
    Serial.println("SimUServeNetworkSettingsManager::loadSettings");
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

    
    this->setDeviceConfigServerPort(doc[D_CONFIG_SERVER_PORT] | DEFAULT_CONFIG_SERVER_PORT);
    Serial.println("Settings Deserialized successfully");
}

#pragma endregion

#pragma region Related to remote(PC) SimUServe server

void SimUServeNetworkSettingsManager::setServerUdpPort(uint16_t const port)
{
    if (this->_networkSettings.ServerUdpPort != port)
    {
        this->_isDirty = true;
        this->_networkSettings.ServerUdpPort = port;
    }
}

uint16_t SimUServeNetworkSettingsManager::getServerUdpPort(void) const
{
    return this->_networkSettings.ServerUdpPort;
}

void SimUServeNetworkSettingsManager::setServerIpAddress(String const &ipAddress)
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

IPAddress const &SimUServeNetworkSettingsManager::getServerIpAddress(void) const
{
    return this->_svrIpAddress;
}

#pragma endregion

#pragma region Related to Device Access Point(Local) server, for setup purposes

void SimUServeNetworkSettingsManager::setDeviceUdpPort(uint16_t const port)
{
    if (this->_networkSettings.DeviceUdpPort != port)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceUdpPort = port;
    }
}

uint16_t SimUServeNetworkSettingsManager::getDeviceUdpPort(void) const
{
    return this->_networkSettings.DeviceUdpPort;
}

void SimUServeNetworkSettingsManager::setDeviceConfigServerPort(uint16_t const port)
{
    if (this->_networkSettings.DeviceConfigServerPort != port)
    {
        this->_isDirty = true;
        this->_networkSettings.DeviceConfigServerPort = port;
    }
}

uint16_t SimUServeNetworkSettingsManager::getDeviceConfigServerPort(void) const
{
    return this->_networkSettings.DeviceConfigServerPort;
}

void SimUServeNetworkSettingsManager::setDeviceApSsid(String const &deviceApSsid)
{
    if (this->_networkSettings.DeviceApSsid != deviceApSsid)
    {
        _isDirty = true;
        this->_networkSettings.DeviceApSsid = String(deviceApSsid);
    }
}

String const &SimUServeNetworkSettingsManager::getDeviceApSsid(void) const
{
    return this->_networkSettings.DeviceApSsid;
}

void SimUServeNetworkSettingsManager::setDeviceApNetworkSecurityKey(String const &apSecurityKey)
{
    if (this->_networkSettings.DeviceApNetworkSecurityKey != apSecurityKey)
    {
        _isDirty = true;
        this->_networkSettings.DeviceApNetworkSecurityKey = String(apSecurityKey);
    }
}

String const &SimUServeNetworkSettingsManager::getDeviceApNetworkSecurityKey(void) const
{
    return this->_networkSettings.DeviceApNetworkSecurityKey;
}

void SimUServeNetworkSettingsManager::setDeviceApHostName(String const &deviceHostName)
{
    if (this->_networkSettings.DeviceApHostName != deviceHostName)
    {
        _isDirty = true;
        this->_networkSettings.DeviceApHostName = String(deviceHostName);
    }
}

String const &SimUServeNetworkSettingsManager::getDeviceApHostName(void) const
{
    return this->_networkSettings.DeviceApHostName;
}

void SimUServeNetworkSettingsManager::setDeviceApIpAddress(String const &deviceApIp)
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

IPAddress const &SimUServeNetworkSettingsManager::getDeviceApIpAddress(void) const
{
    return this->_apIpAddress;
}

void SimUServeNetworkSettingsManager::setDeviceApGatewayIpAddress(String const &deviceApGatewayIp)
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

IPAddress const &SimUServeNetworkSettingsManager::getDeviceApGatewayIpAddress(void) const
{
    return this->_apGatewayIpAddress;
}

void SimUServeNetworkSettingsManager::setDeviceApNetmask(String const &apNetmaskIp)
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

IPAddress const &SimUServeNetworkSettingsManager::getDeviceApNetmask(void) const
{
    return this->_apNetmask;
}

#pragma endregion

#pragma region Related to connecting to main WiFi access point

void SimUServeNetworkSettingsManager::setConnectedNetworkDeviceIpAddress(String const &deviceIp)
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

IPAddress const &SimUServeNetworkSettingsManager::getConnectedNetworkDeviceIpAddress(void) const
{
    return this->_cnDeviceIp;
}

void SimUServeNetworkSettingsManager::setConnectedNetworkGatewayIpAddress(String const &gatewayIp)
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

IPAddress const &SimUServeNetworkSettingsManager::getConnectedNetworkGatewayIpAddress(void) const
{
    return this->_cnGatewayIpAddress;
};

void SimUServeNetworkSettingsManager::setConnectedNetworkNetmask(String const &netmask)
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

IPAddress const &SimUServeNetworkSettingsManager::getConnectedNetworkNetmask(void) const
{
    return this->_cnNetmask;
}

void SimUServeNetworkSettingsManager::setConnectedNetworkSsid(String const &connectedSsid)
{
    if (this->_networkSettings.ConnectedNetworkSsid != connectedSsid)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkSsid = String(connectedSsid);
    }
}

String const &SimUServeNetworkSettingsManager::getConnectedNetworkSsid(void) const
{
    return this->_networkSettings.ConnectedNetworkSsid;
}

void SimUServeNetworkSettingsManager::setConnectedNetworkSecurityKey(String const &securityKey)
{
    if (this->_networkSettings.ConnectedNetworkSecurityKey != securityKey)
    {
        this->_isDirty = true;
        this->_networkSettings.ConnectedNetworkSecurityKey = String(securityKey);
    }
}

String const &SimUServeNetworkSettingsManager::getConnectedNetworkSecurityKey(void) const
{
    return this->_networkSettings.ConnectedNetworkSecurityKey;
}

#pragma endregion

void SimUServeNetworkSettingsManager::setClean(void)
{
    this->_isDirty = false;
}

void SimUServeNetworkSettingsManager::update(NetworkSettings const fromSettings, bool autoPersist)
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

void SimUServeNetworkSettingsManager::saveSettings(void)
{
    if(!this->_isDirty)
    {
        Serial.println("Settings not dirty - not saving");
        return;
    }

    if(!LittleFS.begin())
    {
        Serial.println("Count not mount LittleFS to save settings");
        return;
    }

    // TODO - handle saving this to EEPROM or FS
    File file = LittleFS.open("/config.json", "w");
    
    

}