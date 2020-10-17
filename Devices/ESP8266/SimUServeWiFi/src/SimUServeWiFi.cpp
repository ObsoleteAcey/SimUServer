#include "SimUServeWiFi.h"

SimUServeWiFi::SimUServeWiFi() 
{
    initDefaults();
    // now check and load any settings that have been saved
    SimUServeWiFiSettings loadedSettings;
    if(checkEepromForValue(0, loadedSettings))
    {
        _settings->update(loadedSettings);
    }

    _server = new WiFiServer(_settings->getServerIpAddress(), _settings->getServerPort());
}

SimUServeWiFi::SimUServeWiFi(int serverPort, String const& serverIpAddress)
{
    initDefaults();
    _settings->setServerPort(serverPort);
    _settings->setServerSsid(serverIpAddress);
    _server = new WiFiServer(_settings->getServerIpAddress(), _settings->getServerPort());
}

SimUServeWiFi::~SimUServeWiFi()
{
    delete _settings;
    delete _mdns;
    delete _server;
}

String const& SimUServeWiFi::getWiFiSsid(void) const
{
    return _settings->getConnectedSsid();
}

String const& SimUServeWiFi::getWiFiPassword(void) const
{
    return _settings->getConnectedPassword();
}

template <typename T>
bool SimUServeWiFi::checkEepromForValue(int startOffset, T &retrievedValue)
{
    EEPROM.get<T>(startOffset, retrievedValue);
    
    return retrievedValue == nullptr ? true : false;
}

template <typename T>
void SimUServeWiFi::writeValueToEeprom(int startOffset, T const& valueToSave)
{
    EEPROM.put<T>(startOffset, valueToSave);
}

void SimUServeWiFi::initDefaults()
{
   _settings->setServerPort(DEFAULT_SERVER_PORT);
   _settings->setServerIpAddress(DEFAULT_SERVER_IP);
   _settings->setServerSsid(DEFAULT_SERVER_SSID);
   _settings->setServerPassword(DEFAULT_SERVER_PASSWORD);
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

void SimUServeWiFi::startMdns(void)
{
    if(!_mdns->begin(SERVER_LOCAL_ADDRESS, _settings->getServerIpAddress())) {

    };
}

void SimUServeWiFi::setupAccessPoint(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int numberOfNetworks = WiFi.scanNetworks();

    if(numberOfNetworks > 0)
    {
        String htmlSnippet = "<ul>";
        for (int i = 0; i < numberOfNetworks; ++i)
        {
            // Print SSID and RSSI for each network found
            htmlSnippet += "<li>";
            htmlSnippet += i + 1;
            htmlSnippet += ": ";
            htmlSnippet += WiFi.SSID(i);
            htmlSnippet += " (";
            htmlSnippet += WiFi.RSSI(i);
            htmlSnippet += ")";
            htmlSnippet += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
            htmlSnippet += "</li>";
        }
        htmlSnippet += "</ul>";
        delay(100);
        
        WiFi.softAP(_settings->getServerSsid(), _settings->getServerPassword());
    }
}

WifiNetwork* const SimUServeWiFi::getAvailableWifiNetworks(void)
{
    int numberOfNetworks = WiFi.scanNetworks();
    _availableNetworks = new WifiNetwork[numberOfNetworks];

    for (int i = 0; i < numberOfNetworks; ++i)
    {
        _availableNetworks[i] = new WifiNetwork(i, WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
    }

    return _availableNetworks;
}