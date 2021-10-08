/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeNetworkSettings.h"

#pragma region Constructors/Destructors

SimUServeNetworkSettings* SimUServeNetworkSettings::_settings = nullptr;

SimUServeNetworkSettings* SimUServeNetworkSettings::getSettings(void)
{
    if (_settings == nullptr)
    {
        _settings = new SimUServeNetworkSettings();
    }

    return _settings;
}

SimUServeNetworkSettings::~SimUServeNetworkSettings()
{
    
}

#pragma endregion

#pragma region Related to remote (PC) SimUServe server

void SimUServeNetworkSettings::setServerUdpPort(uint16_t const port)
{
    if(this->_serverUdpPort != port)
    {
        this->_isDirty = true;
        this->_serverUdpPort = port;
    }
}

uint16_t SimUServeNetworkSettings::getServerUdpPort(void) const
{
    return this->_serverUdpPort;
}

void SimUServeNetworkSettings::setServerIpAddress(String const& ipAddress)
{
    if(this->_serverIpAddress != ipAddress)
    {
        this->_isDirty = true;
        this->_serverIpAddress = String(ipAddress);
        if (!this->_svrIpAddress)
        {
            this->_svrIpAddress = IPAddress();
        }
        this->_svrIpAddress.fromString(ipAddress);
    }
}

IPAddress const& SimUServeNetworkSettings::getServerIpAddress(void) const
{
    return this->_svrIpAddress;
}


#pragma endregion

#pragma region Related to Device Access Point (Local) server, for setup purposes

void SimUServeNetworkSettings::setDeviceUdpPort(uint16_t const port)
{
    if(this->_deviceUdpPort != port)
    {
        this->_isDirty = true;
        this->_deviceUdpPort = port;
    }
}

uint16_t SimUServeNetworkSettings::getDeviceUdpPort(void) const
{
    return this->_deviceUdpPort;
}

void SimUServeNetworkSettings::setDeviceConfigServerPort(uint16_t const port)
{
    if(this->_deviceConfigServerPort != port)
    {
        this->_isDirty = true;
        this->_deviceConfigServerPort = port;
    }
}

uint16_t SimUServeNetworkSettings::getDeviceConfigServerPort(void) const
{
    return this->_deviceConfigServerPort;
}

void SimUServeNetworkSettings::setDeviceApSsid(String const& deviceApSsid)
{
    if(this->_deviceApSsid != deviceApSsid)
    {
        _isDirty = true;
        this->_deviceApSsid = String(deviceApSsid);
    }
}

String const& SimUServeNetworkSettings::getDeviceApSsid(void) const
{
    return this->_deviceApSsid;
}

void SimUServeNetworkSettings::setDeviceApNetworkSecurityKey(String const& apSecurityKey)
{
    if(this->_deviceApNetworkSecurityKey != apSecurityKey)
    {
        _isDirty = true;
        this->_deviceApNetworkSecurityKey = String(apSecurityKey);
    }
}

String const& SimUServeNetworkSettings::getDeviceApNetworkSecurityKey(void) const
{
    return this->_deviceApNetworkSecurityKey;
}

void SimUServeNetworkSettings::setDeviceApHostName(String const& deviceHostName) 
{
    if (this->_deviceApHostName != deviceHostName)
    {
        _isDirty = true;
        this->_deviceApHostName = String(deviceHostName);
    }
}

String const& SimUServeNetworkSettings::getDeviceApHostName(void) const
{
    return this->_deviceApHostName;
}

void SimUServeNetworkSettings::setDeviceApIpAddress(String const& deviceApIp)
{
    if (this->_deviceApIpAddress != deviceApIp)
    {
        this->_isDirty = true;
        this->_deviceApIpAddress = String(deviceApIp);
        if (!this->_apIpAddress)
        {
            this->_apIpAddress = IPAddress();
        }
        this->_apIpAddress.fromString(deviceApIp);
    }
}

IPAddress const& SimUServeNetworkSettings::getDeviceApIpAddress(void) const
{
    return this->_apIpAddress;
}

void SimUServeNetworkSettings::setDeviceApGatwayIpAddress(String const& deviceApGatewayIp)
{
    if (this->_deviceApGatewayIpAddress != deviceApGatewayIp)
    {
        this->_isDirty = true;
        this->_deviceApGatewayIpAddress = String(deviceApGatewayIp);
        if (!this->_apGatewayIpAddress)
        {
            this->_apGatewayIpAddress = IPAddress();
        }
        this->_apGatewayIpAddress.fromString(deviceApGatewayIp);
    }
}

IPAddress const& SimUServeNetworkSettings::getDeviceApGatwayIpAddress(void) const
{
    return this->_apGatewayIpAddress;
}

void SimUServeNetworkSettings::setDeviceApNetmask(String const& apNetmaskIp)
{
    if (this->_deviceApNetmask != apNetmaskIp)
    {
        this-> _isDirty = true;
        this-> _deviceApNetmask = String(apNetmaskIp);
        if (!this->_apNetmask)
        {
            this->_apNetmask = IPAddress();
        }
        this->_apNetmask.fromString(apNetmaskIp);
    }
}

IPAddress const& SimUServeNetworkSettings::getDeviceApNetmask(void) const
{
    return this->_apNetmask;
}

#pragma endregion


  #pragma region Related to connecting to main WiFi access point


  void SimUServeNetworkSettings::setConnectedNetworkDeviceIpAddress(String const& deviceIp)
  {
    if (this->_connectedNetworkDeviceIp != deviceIp)
        {
            this-> _isDirty = true;
            this-> _connectedNetworkDeviceIp = String(deviceIp);
            if(!this->_cnDeviceIp)
            {
                this->_cnDeviceIp = IPAddress();
            }
            this->_cnDeviceIp.fromString(deviceIp);
        }
  }

    IPAddress const& SimUServeNetworkSettings::getConnectedNetworkDeviceIpAddress(void) const
    {
        return this->_cnDeviceIp;
    }

  void SimUServeNetworkSettings::setConnectedNetworkGatewayIpAddress(String const& gatewayIp)
  {
    if (this->_connectedNetworkGatewayIpAddress != gatewayIp)
    {
        this-> _isDirty = true;
        this-> _connectedNetworkGatewayIpAddress = String(gatewayIp);
        if(!this->_cnGatewayIpAddress)
        {
            this->_cnGatewayIpAddress = IPAddress();
        }
        this->_cnGatewayIpAddress.fromString(gatewayIp);
    }
  };

    IPAddress const& SimUServeNetworkSettings::getConnectedNetworkGatewayIpAddress(void) const
    {
        return this->_cnGatewayIpAddress;
    };

    void SimUServeNetworkSettings::setConnectedNetworkNetmask(String const& netmask)
    {
        if (this->_connectedNetworkNetmask != netmask)
        {
            this-> _isDirty = true;
            this-> _connectedNetworkNetmask = String(netmask);
            if(!this->_cnNetmask)
            {
                this->_cnNetmask = IPAddress();
            }
            this->_cnNetmask.fromString(netmask);
        }
    };

    IPAddress const& SimUServeNetworkSettings::getConnectedNetworkNetmask(void) const
    {
        return this->_cnNetmask;
    }

void SimUServeNetworkSettings::setConnectedNetworkSsid(String const& connectedSsid)
{
    if(this->_connectedNetworkSsid != connectedSsid)
    {
        this->_isDirty = true;
        this->_connectedNetworkSsid = String(connectedSsid);
    }
}

String const& SimUServeNetworkSettings::getConnectedNetworkSsid(void) const
{
    return this->_connectedNetworkSsid;
}

void SimUServeNetworkSettings::setConnectedNetworkSecurityKey(String const& securityKey) 
{
    if(this->_connectedNetworkSecurityKey != securityKey)
    {
        this->_isDirty = true;
        this->_connectedNetworkSecurityKey = String(securityKey);
    }
}

String const& SimUServeNetworkSettings::getConnectedNetworkSecurityKey(void) const
{
    return this->_connectedNetworkSecurityKey;
}

  #pragma endregion




void SimUServeNetworkSettings::setClean(void)
{
    this->_isDirty = false;
}

void SimUServeNetworkSettings::update(SimUServeNetworkSettings const& fromSettings, bool autoPersist)
{
    // TODO - some kind fo detection for default vs loaded vs dirty
    // set up server settings
    this->setServerIpAddress(fromSettings.getServerIpAddress().toString());
    this->setServerUdpPort(fromSettings.getServerUdpPort());

    // set up AP settings
    this->setDeviceApSsid(fromSettings.getDeviceApSsid());
    this->setDeviceApSsid(fromSettings.getDeviceApSsid());
    this->setDeviceApIpAddress(fromSettings.getDeviceApIpAddress().toString());
    this->setDeviceApNetworkSecurityKey(fromSettings.getDeviceApNetworkSecurityKey());
    this->setDeviceApGatwayIpAddress(fromSettings.getDeviceApGatwayIpAddress().toString());
    this->setDeviceApHostName(fromSettings.getDeviceApHostName());

    if (autoPersist)
    {
        this->saveSettings();
    }
}

void SimUServeNetworkSettings::saveSettings(void)
{
    // TODO - handle saving this to EEPROM or FS
}