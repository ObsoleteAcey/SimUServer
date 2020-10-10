#include "SimUServeWiFi.h"

SimUServeWiFi::SimUServeWiFi(int serverPort, String serverIpAddress)
{

}

String SimUServeWiFi::getWiFiSsid()
{
    if(this->_wifiSsid == NULL || this->_wifiSsid.length() == 0) {

    }
    
    return String();
}

String SimUServeWiFi::getWiFiPassword()
{
    return String();
}

bool SimUServeWiFi::checkEepromForValue(int startOffset, int length, String& retrievedValue)
{
    for (int i = startOffset; i < length; ++i)
    {
        retrievedValue += char(EEPROM.read(i));
    }

    return retrievedValue.length() > 0 ? true : false;
}