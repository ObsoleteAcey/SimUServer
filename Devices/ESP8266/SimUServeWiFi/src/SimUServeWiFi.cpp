#include "SimUServeWiFi.h"

SimUServeWiFi::SimUServeWiFi() 
{

}

SimUServeWiFi::SimUServeWiFi(int serverPort, String serverIpAddress)
{
    _serverPort = serverPort;
    _serverIpAddress = String(serverIpAddress);
}

String SimUServeWiFi::getWiFiSsid()
{
    if(this->_wifiSsid == NULL || this->_wifiSsid.length() == 0) {

    }
    
    return String();
}

String SimUServeWiFi::getWiFiPassword()
{
    if(this->_wifiPassword == NULL || this->_wifiPassword.length() == 0) {

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

void SimUServeWiFi::write