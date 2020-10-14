#include "SimUServeWiFiSettings.h"

SimUServeWiFiSettings::~SimUServeWiFiSettings()
{
    delete _serverIpAddress;
    delete _serverSsid;
    delete _serverPassword;
    delete _connectedSsid;
    delete _connectedPassword;
    delete _ipAddress;
    delete _deviceHostName;
}

void SimUServeWiFiSettings::update(SimUServeWiFiSettings const& fromSettings)
{
    // TODO - some kind fo detection for default vs loaded vs dirty
    if(getConnectedPassword() != fromSettings.getConnectedPassword())
    {
        setConnectedPassword(fromSettings.getConnectedPassword());
    }

    if(getConnectedSsid() != fromSettings.getConnectedSsid())
    {
        setConnectedSsid(fromSettings.getConnectedSsid());
    }

    if(getServerIpAddress().toString() != fromSettings.getServerIpAddress().toString()) 
    {
        setServerIpAddress(fromSettings.getServerIpAddress().toString());
    }

    if(getServerPort() != fromSettings.getServerPort()) 
    {
        setServerPort(fromSettings.getServerPort());
    }

    if(getServerPassword() != fromSettings.getServerPassword()) 
    {
        setServerPassword(fromSettings.getServerPassword());
    }

    if(getServerSsid() != fromSettings.getServerSsid()) 
    {
        setServerSsid(fromSettings.getServerSsid());
    }
}

void SimUServeWiFiSettings::setServerPort(int port)
{
    _serverPort = port;
}

int SimUServeWiFiSettings::getServerPort(void) const
{
    return _serverPort;
}

void SimUServeWiFiSettings::setServerIpAddress(String const& ipAddress)
{
    delete _serverIpAddress;
    delete _ipAddress;
    _serverIpAddress = new String(ipAddress);
    _ipAddress = new IPAddress();
    _ipAddress->fromString(ipAddress);
}

IPAddress const& SimUServeWiFiSettings::getServerIpAddress(void) const
{
    return *_ipAddress;
}

void SimUServeWiFiSettings::setServerSsid(String const& serverSsid)
{
    delete _serverSsid;
    _serverSsid = new String(serverSsid);
}

String const& SimUServeWiFiSettings::getServerSsid(void) const
{
    return *_serverSsid;
}

void SimUServeWiFiSettings::setServerPassword(String const& serverPassword)
{
    delete _serverPassword;
    _serverPassword = new String(serverPassword);
}

String const& SimUServeWiFiSettings::getServerPassword(void) const
{
    return *_serverPassword;
}

void SimUServeWiFiSettings::setConnectedSsid(String const& connectedSsid)
{
    delete _connectedSsid;
    _connectedSsid = new String(connectedSsid);
}

String const& SimUServeWiFiSettings::getConnectedSsid(void) const
{
    return *_connectedSsid;
}

void SimUServeWiFiSettings::setConnectedPassword(String const& connectedPassword) 
{
    delete _connectedPassword;
    _connectedPassword = new String(connectedPassword);
}

String const& SimUServeWiFiSettings::getConnectedPassword(void) const
{
    return *_connectedPassword;
}

void SimUServeWiFiSettings::setDeviceHostName(String const& deviceHostName) 
{
    delete _deviceHostName;
    _deviceHostName = new String(deviceHostName);
}

String const& SimUServeWiFiSettings::getDeviceHostName(void) const
{
    return *_deviceHostName;
}