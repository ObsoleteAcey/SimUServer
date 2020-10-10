#include "SimUServeWiFi.h"

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
        
    }
    
    return String();
}

bool SimUServeWiFi::checkEepromForValue(int startOffset, int length, String& retrievedValue)
{
    for (int i = startOffset; i < length; ++i)
    {
        char storedValue = char(EEPROM.read(i));

        retrievedValue += storedValue;
    }

    return retrievedValue.length() > 0 ? true : false;
}

void SimUServeWiFi::writeValueToEeprom(int startOffset, int length, String valueToSave)
{
    
}

void SimUServeWiFi::initDefaults()
{
   _settings.serverPort = DEFAULT_SERVER_PORT;
   _settings.serverIpAddress = DEFAULT_SERVER_IP;
   _settings.serverSsid = DEFAULT_SERVER_SSID;
   _settings.serverPassword = DEFAULT_SERVER_PASSWORD;
}