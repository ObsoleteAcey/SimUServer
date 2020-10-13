#include "SimUServeWiFi.h"

void SimUServeWiFiSettings::update(const SimUServeWiFiSettings* fromSettings)
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

IPAddress SimUServeWiFiSettings::getServerIpAddress()
{
    IPAddress* ipAddress = new IPAddress();
    if(ipAddress->fromString(serverIpAddress))
    {
        return ipAddress;
    }
}

SimUServeWiFi::SimUServeWiFi() 
{
    initDefaults();
    // now check and load any settings that have been saved
    SimUServeWiFiSettings loadedSettings;
    if(checkEepromForValue(0, loadedSettings))
    {
        _settings->update(loadedSettings);
    }

    _server = new WiFiServer(_settings->getServerIpAddress(), _settings->serverPort);
}

SimUServeWiFi::SimUServeWiFi(int serverPort, String serverIpAddress)
{
    initDefaults();
    _settings->setServerPort(serverPort);
    _settings->setServerSsid(serverIpAddress);
    _server = WiFiServer(_settings.getServerIpAddress(), _settings.serverPort);
}

String SimUServeWiFi::getWiFiSsid()
{
    return _settings.connectedSsid;
}

String SimUServeWiFi::getWiFiPassword()
{
    return _settings.connectedPassword;
}

template <typename T>
bool SimUServeWiFi::checkEepromForValue(int startOffset, T &retrievedValue)
{
    EEPROM.get<T>(startOffset, retrievedValue);
    
    return retrievedValue == nullptr ? true : false;
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

bool SimUServeWiFi::testWifiConnection() 
{
    int waitRetryCounter = 0;
    while ( waitRetryCounter < 20 ) 
    {
        if (WiFi.status() == WL_CONNECTED)
        { 
            return(true);
        } 
        delay(500);
        Serial.print(WiFi.status());    
        waitRetryCounter++;
    }
    Serial.println("Connect timed out, opening AP");
    return(false);
}