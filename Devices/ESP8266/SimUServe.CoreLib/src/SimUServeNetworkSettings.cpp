/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeNetworkSettings.h"

SimUServeNetworkSettings* SimUServeNetworkSettings::_settings = nullptr;

SimUServeNetworkSettings* SimUServeNetworkSettings::GetSettings(void)
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

void SimUServeNetworkSettings::update(SimUServeNetworkSettings const& fromSettings)
{
    // TODO - some kind fo detection for default vs loaded vs dirty
    setConnectedPassword(fromSettings.getConnectedPassword());
    setConnectedSsid(fromSettings.getConnectedSsid());
    setServerIpAddress(fromSettings.getServerIpAddress().toString());
    setServerPort(fromSettings.getServerPort());
    setServerPassword(fromSettings.getServerPassword());
    setServerSsid(fromSettings.getServerSsid());
}

void SimUServeNetworkSettings::setServerPort(int const port)
{
    if(_serverPort != port)
    {
        _isDirty = true;
        _serverPort = port;
    }
}

int SimUServeNetworkSettings::getServerPort(void) const
{
    return _serverPort;
}

void SimUServeNetworkSettings::setServerIpAddress(String const& ipAddress)
{
    if(_serverIpAddress != ipAddress)
    {
        _isDirty = true;
        _serverIpAddress = String(ipAddress);
        if (!_ipAddress)
        {
            _ipAddress = IPAddress();
        }
        _ipAddress.fromString(ipAddress);
    }
}

IPAddress const& SimUServeNetworkSettings::getServerIpAddress(void) const
{
    return _ipAddress;
}

void SimUServeNetworkSettings::setServerGatewayIpAddress(String const& ipAddress)
{
    if(_serverGatewayIpAddress != ipAddress)
    {
        _isDirty = true;
        _serverGatewayIpAddress = String(ipAddress);
        if (!_gatewayIpAddress)
        {
            _gatewayIpAddress = IPAddress();
        }
        _gatewayIpAddress.fromString(ipAddress);
    }
}

IPAddress const& SimUServeNetworkSettings::getServerGatewayIpAddress(void) const
{
    return _gatewayIpAddress;
}

void SimUServeNetworkSettings::setServerNetmask(String const& ipAddress)
{
    if(_serverNetmask != ipAddress)
    {
        _isDirty = true;
        _serverNetmask = String(ipAddress);
        if (!_netmask)
        {
            _netmask = IPAddress();
        }
        _netmask.fromString(ipAddress);
    }
}

IPAddress const& SimUServeNetworkSettings::getServerNetmask(void) const
{
    return _netmask;
}

void SimUServeNetworkSettings::setServerSsid(String const& serverSsid)
{
    if(_serverSsid != serverSsid)
    {
        _isDirty = true;
        _serverSsid = String(serverSsid);
    }
}

String const& SimUServeNetworkSettings::getServerSsid(void) const
{
    return _serverSsid;
}

void SimUServeNetworkSettings::setServerPassword(String const& serverPassword)
{
    if(_serverPassword != serverPassword)
    {
        _isDirty = true;
        _serverPassword = String(serverPassword);
    }
}

String const& SimUServeNetworkSettings::getServerPassword(void) const
{
    return _serverPassword;
}

void SimUServeNetworkSettings::setConnectedSsid(String const& connectedSsid)
{
    if(_connectedSsid != connectedSsid)
    {
        _isDirty = true;
        _connectedSsid = String(connectedSsid);
    }
}

String const& SimUServeNetworkSettings::getConnectedSsid(void) const
{
    return _connectedSsid;
}

void SimUServeNetworkSettings::setConnectedPassword(String const& connectedPassword) 
{
    if(_connectedPassword != connectedPassword)
    {
        _isDirty = true;
        _connectedPassword = String(connectedPassword);
    }
}

String const& SimUServeNetworkSettings::getConnectedPassword(void) const
{
    return _connectedPassword;
}

void SimUServeNetworkSettings::setDeviceHostName(String const& deviceHostName) 
{
    if(_deviceHostName != deviceHostName)
    {
        _isDirty = true;
        _deviceHostName = String(deviceHostName);
    }
}

String const& SimUServeNetworkSettings::getDeviceHostName(void) const
{
    return _deviceHostName;
}

void SimUServeNetworkSettings::setClean(void)
{
    _isDirty = false;
}