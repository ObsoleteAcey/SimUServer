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
}

SimUServeWiFi::SimUServeWiFi(int serverPort, String const& serverIpAddress)
{
    initDefaults();
    _settings->setServerPort(serverPort);
    _settings->setServerSsid(serverIpAddress);
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
    retrievedValue = EEPROM.get<T>(startOffset, retrievedValue);
    
    return true;
}

template <typename T>
void SimUServeWiFi::writeValueToEeprom(int startOffset, T const& valueToSave)
{
    EEPROM.put<T>(startOffset, valueToSave);
}

void SimUServeWiFi::initServices(void) 
{ 
    Serial.println("SimUServeWiFi::initServices");
    if(!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
    }
    setupAccessPoint();
    startMdns();
}

void SimUServeWiFi::initDefaults()
{
    Serial.println("SimUServeWiFi::initDefaults");
   _settings->setServerPort(DEFAULT_SERVER_PORT);
   _settings->setServerIpAddress(DEFAULT_SERVER_IP);
   _settings->setServerSsid(DEFAULT_SERVER_SSID + String(ESP.getChipId()));
   _settings->setServerPassword(DEFAULT_SERVER_PASSWORD + String(ESP.getChipId());
   numberOfNetworks = 0;
}

bool SimUServeWiFi::testWifiConnection() 
{
    Serial.println("SimUServeWiFi::testWifiConnection");
    int waitRetryCounter = 0;
    while ( waitRetryCounter < 20 ) 
    {
        if (WiFi.status() == WL_CONNECTED)
        { 
            Serial.println("Wifi connection established");
            return(true);
        } 
        delay(500);
        Serial.print(WiFi.status());    
        waitRetryCounter++;
    }
    Serial.println("Connect timed out");
    return(false);
}

void SimUServeWiFi::startMdns(void)
{
    Serial.println("SimUServeWiFi::startMdns");
    if(!_mdns->begin(SERVER_LOCAL_ADDRESS, _settings->getServerIpAddress())) {

    };
    launchWebServer();
    Serial.println("Adding http service to MDNS on port " + String(_settings->getServerPort()));
    _mdns->addService("http", "tcp", _settings->getServerPort());
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
    Serial.println("SimUServeWiFi::getAvailableWifiNetworks");
    numberOfNetworks = WiFi.scanNetworks();
    if(_availableNetworks)
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
    Serial.println("SimUServeWiFi::launchWebServer");
    if(!_server)
    {
        Serial.println("SimUServeWiFi::launchWebServer");
        _server = new AsyncWebServer(_settings->getServerPort());
    }
    // set up the routes
    _server->on("/", HTTP_GET, SimUServeWiFi::handleRootGet);
    _server->on("/network/all", HTTP_GET,  SimUServeWiFi::handleRefreshNetworksGet);
    _server->on("/network", HTTP_POST, SimUServeWiFi::handleSaveNetwork);
    _server->onNotFound(SimUServeWiFi::handleNotFound);
    _server->begin();
}

void SimUServeWiFi::checkForWebRequests(void)
{
    _server->handleClient();
    _mdns->update();
}

void SimUServeWiFi::handleRootGet(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "index.html", "text/html");
}

void SimUServeWiFi::handleRefreshNetworksGet(AsyncWebServerRequest *request)
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

void SimUServeWiFi::handleSaveNetwork(AsyncWebServerRequest *request)
{
    if(!request->hasArg("ssid") || !request->hasArg("password") ||
        request->arg("ssid") == NULL || request->arg("password") == NULL)
    {
        // The request is invalid, so send HTTP status 400
        request->send(400, "text/plain", "400: Invalid Request");         
        return;
    }

    String ssid =  request->arg("ssid");
    String password = request->arg("password");

    WiFi.begin(ssid, password);
    if(testWifiConnection()) {
        // save settings here and respond with all good

        request->send(200, "Connection successful and settings saved");
        return;
    }

    request->send(400, "text/plain", "Password was incorrect or network connection failed.  Please try again");    
}

void SimUServeWiFi::handleNotFound(AsyncWebServerRequest *request)
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += request->uri();
    message += "\nMethod: ";
    message += ( request->method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += request->args();
    message += "\n";

    for ( uint8_t i = 0; i < request->args(); i++ ) {
        message += " " + request->argName ( i ) + ": " + request->arg ( i ) + "\n";
    }
    request->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    request->sendHeader("Pragma", "no-cache");
    request->sendHeader("Expires", "-1");
    request->sendHeader("Content-Length", String(message.length()));
    request->send ( 404, "text/plain", message );
}