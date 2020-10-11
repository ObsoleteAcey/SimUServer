#include "SimUServeWiFi.h"

void SimUServeWiFiSettings::update(SimUServeWiFiSettings fromSettings)
{
    // TODO - some kind fo detection for default vs loaded vs dirty
    if(connectedPassword != fromSettings.connectedPassword) 
    {
        connectedPassword = fromSettings.connectedPassword;
    }

    if(connectedSsid != fromSettings.connectedSsid) 
    {
        connectedSsid = fromSettings.connectedSsid;
    }

    if(serverIpAddress != fromSettings.serverIpAddress) 
    {
        serverIpAddress = fromSettings.serverIpAddress;
    }

    if(serverPort != fromSettings.serverPort) 
    {
        serverPort = fromSettings.serverPort;
    }

    if(serverPassword != fromSettings.serverPassword) 
    {
        serverPassword = fromSettings.serverPassword;
    }

    if(serverSsid != fromSettings.serverSsid) 
    {
        serverSsid = fromSettings.serverSsid;
    }
}

SimUServeWiFi::SimUServeWiFi() 
{
    initDefaults();
}

SimUServeWiFi::SimUServeWiFi(int serverPort, String serverIpAddress)
{
    initDefaults();
    _settings.serverPort = serverPort;
    _settings.serverSsid = String(serverIpAddress);
}

String SimUServeWiFi::getWiFiSsid()
{
    if(this->_settings.connectedSsid == NULL || this->_settings.connectedSsid.length() == 0) {
        
    }
    
    return String();
}

String SimUServeWiFi::getWiFiPassword()
{
    if(this->_settings.connectedPassword == NULL || this->_settings.connectedPassword.length() == 0) {
        SimUServeWiFiSettings retrievedSettings;
        if(checkEepromForValue(0, retrievedSettings))
        {
            
        }
    }
    
    return String();
}

template <typename T>
bool SimUServeWiFi::checkEepromForValue(int startOffset, T &retrievedValue)
{
    EEPROM.get<T>(startOffset, retrievedValue);
    
    return retrievedValue == NULL ? true : false;
}

template <typename T>
void SimUServeWiFi::writeValueToEeprom(int startOffset, T valueToSave)
{
    EEPROM.put<T>(startOffset, valueToSave);
}

void SimUServeWiFi::initDefaults()
{
   _settings.serverPort = DEFAULT_SERVER_PORT;
   _settings.serverIpAddress = DEFAULT_SERVER_IP;
   _settings.serverSsid = DEFAULT_SERVER_SSID;
   _settings.serverPassword = DEFAULT_SERVER_PASSWORD;
}