#include "SimUServeWiFiSettings.h"

SimUServeWiFiSettings::~SimUServeWiFiSettings()
{
    delete _ipAddress;
}

void SimUServeWiFiSettings::update(SimUServeWiFiSettings const& fromSettings)
{
    // TODO - some kind fo detection for default vs loaded vs dirty
    setConnectedPassword(fromSettings.getConnectedPassword());
    setConnectedSsid(fromSettings.getConnectedSsid());
    setServerIpAddress(fromSettings.getServerIpAddress().toString());
    setServerPort(fromSettings.getServerPort());
    setServerPassword(fromSettings.getServerPassword());
    setServerSsid(fromSettings.getServerSsid());
}

void SimUServeWiFiSettings::setServerPort(int const port)
{
    if(_serverPort != port)
    {
        _isDirty = true;
        _serverPort = port;
    }
}

int SimUServeWiFiSettings::getServerPort(void) const
{
    return _serverPort;
}

void SimUServeWiFiSettings::setServerIpAddress(String const& ipAddress)
{
    if(_serverIpAddress != ipAddress)
    {
        _isDirty = true;
        _serverIpAddress = String(ipAddress);
        if (!_ipAddress)
        {
            _ipAddress = new IPAddress();
        }
        _ipAddress->fromString(ipAddress);
    }
}

IPAddress const& SimUServeWiFiSettings::getServerIpAddress(void) const
{
    return *_ipAddress;
}

void SimUServeWiFiSettings::setServerGatewayIpAddress(String const& ipAddress)
{
    if(_serverGatewayIpAddress != ipAddress)
    {
        _isDirty = true;
        _serverGatewayIpAddress = String(ipAddress);
        if (!_gatewayIpAddress)
        {
            _gatewayIpAddress = new IPAddress();
        }
        _gatewayIpAddress->fromString(ipAddress);
    }
}

IPAddress const& SimUServeWiFiSettings::getServerGatewayIpAddress(void) const
{
    return *_gatewayIpAddress;
}

void SimUServeWiFiSettings::setServerNetmask(String const& ipAddress)
{
    if(_serverNetmask != ipAddress)
    {
        _isDirty = true;
        _serverNetmask = String(ipAddress);
        if (!_netmask)
        {
            _netmask = new IPAddress();
        }
        _netmask->fromString(ipAddress);
    }
}

IPAddress const& SimUServeWiFiSettings::getServerNetmask(void) const
{
    return *_netmask;
}

void SimUServeWiFiSettings::setServerSsid(String const& serverSsid)
{
    if(_serverSsid != serverSsid)
    {
        _isDirty = true;
        _serverSsid = String(serverSsid);
    }
}

String const& SimUServeWiFiSettings::getServerSsid(void) const
{
    return _serverSsid;
}

void SimUServeWiFiSettings::setServerPassword(String const& serverPassword)
{
    if(_serverPassword != serverPassword)
    {
        _isDirty = true;
        _serverPassword = String(serverPassword);
    }
}

String const& SimUServeWiFiSettings::getServerPassword(void) const
{
    return _serverPassword;
}

void SimUServeWiFiSettings::setConnectedSsid(String const& connectedSsid)
{
    if(_connectedSsid != connectedSsid)
    {
        _isDirty = true;
        _connectedSsid = String(connectedSsid);
    }
}

String const& SimUServeWiFiSettings::getConnectedSsid(void) const
{
    return _connectedSsid;
}

void SimUServeWiFiSettings::setConnectedPassword(String const& connectedPassword) 
{
    if(_connectedPassword != connectedPassword)
    {
        _isDirty = true;
        _connectedPassword = String(connectedPassword);
    }
}

String const& SimUServeWiFiSettings::getConnectedPassword(void) const
{
    return _connectedPassword;
}

void SimUServeWiFiSettings::setDeviceHostName(String const& deviceHostName) 
{
    if(_deviceHostName != deviceHostName)
    {
        _isDirty = true;
        _deviceHostName = String(deviceHostName);
    }
}

String const& SimUServeWiFiSettings::getDeviceHostName(void) const
{
    return _deviceHostName;
}

void SimUServeWiFiSettings::setClean(void)
{
    _isDirty = false;
}