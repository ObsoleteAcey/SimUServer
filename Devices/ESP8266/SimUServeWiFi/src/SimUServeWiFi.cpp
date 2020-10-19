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

    _server = new ESP8266WebServer(_settings->getServerIpAddress(), _settings->getServerPort());
}

SimUServeWiFi::SimUServeWiFi(int serverPort, String const& serverIpAddress)
{
    initDefaults();
    _settings->setServerPort(serverPort);
    _settings->setServerSsid(serverIpAddress);
    _server = new ESP8266WebServer(_settings->getServerIpAddress(), _settings->getServerPort());
}

SimUServeWiFi::~SimUServeWiFi()
{
    delete _settings;
    delete _mdns;
    delete _server;
    delete[] _availableNetworks;
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
   numberOfNetworks = 0;
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
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.softAP(_settings->getServerSsid(), _settings->getServerPassword());
}

WifiNetwork* const SimUServeWiFi::getAvailableWifiNetworks(void)
{
    numberOfNetworks = WiFi.scanNetworks();
    if(_availableNetworks != nullptr)
    {
        delete[] _availableNetworks;
    }

    _availableNetworks = new WifiNetwork[numberOfNetworks];

    for (int i = 0; i < numberOfNetworks; i++)
    {
        _availableNetworks[i] = WifiNetwork(i, WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
    }

    return _availableNetworks;
}

void SimUServeWiFi::launchWebServer(void)
{
    // set up the routes
    _server->on("/", HTTP_GET, handleRootGet);
    _server->on("/refreshnetworks", HTTP_GET, [](AsyncWebServerRequest *request){handleRefreshNetworksGet});
    _server->begin();
}

void SimUServeWiFi::checkForWebRequests(void)
{
    _server->handleClient();
    
}

void SimUServeWiFi::handleRootGet(void)
{

}

void SimUServeWiFi::handleRefreshNetworksGet(void)
{
    auto* availableNetworks = getAvailableWifiNetworks();
    String returnJson;
    returnJson = "{[";
    for (int i = 0; i < numberOfNetworks; i++)
    {
        returnJson.concat(availableNetworks[i].toJson());
        if(i < numberOfNetworks - 1) 
        {
            returnJson.concat(",");
        }
    }

    returnJson.concat("]}");
    _server->send(200, "application/json", returnJson);
}