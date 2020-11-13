/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#include "SimUServeWiFi.h"

SimUServeWiFi::SimUServeWiFi() 
{
    Serial.println("SimUServeWiFi::SimUServeWiFi");
    initDefaults();
    // now check and load any settings that have been saved
}

SimUServeWiFi::SimUServeWiFi(int serverPort, String const& serverSsid)
{
    Serial.println("SimUServeWiFi::SimUServeWiFi(" + String(serverPort) +", " + serverSsid + ")");
    initDefaults();
    _settings->setServerPort(serverPort);
    _settings->setServerSsid(serverSsid);
}

SimUServeWiFi::~SimUServeWiFi()
{
    delete _settings;
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
    if(!_settings) 
    {
        _settings = new SimUServeWiFiSettings();
    }
   _settings->setServerPort(DEFAULT_SERVER_PORT);
   _settings->setServerIpAddress(DEFAULT_SERVER_IP);
   _settings->setServerGatewayIpAddress(DEFAULT_SERVER_GATEWAY);
   _settings->setServerNetmask(DEFAULT_SERVER_NETMASK);
   _settings->setServerSsid(DEFAULT_SERVER_SSID + String(ESP.getChipId()));
   _settings->setServerPassword(DEFAULT_SERVER_PASSWORD + String(ESP.getChipId()));
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
        Serial.print("WiFi Status: " + WiFi.status());    
        waitRetryCounter++;
    }
    Serial.println("Connect timed out");
    return(false);
}

void SimUServeWiFi::startMdns(void)
{
    Serial.println("SimUServeWiFi::startMdns");

    if(!MDNS.begin(SERVER_LOCAL_ADDRESS, _settings->getServerIpAddress())) {

    };
    launchWebServer();
    Serial.println("Adding http service to MDNS on port " + String(_settings->getServerPort()));
    MDNS.addService("http", "tcp", _settings->getServerPort());
}

void SimUServeWiFi::setupAccessPoint(void)
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Starting access point on" + _settings->getServerSsid() + " using password " + _settings->getServerPassword());
    WiFi.softAPConfig(_settings->getServerIpAddress(), _settings->getServerGatewayIpAddress(),
        _settings->getServerNetmask());
    WiFi.softAP(_settings->getServerSsid(), _settings->getServerPassword());
}

WifiNetwork* const SimUServeWiFi::getAvailableWifiNetworks(void)
{
    Serial.println("SimUServeWiFi::getAvailableWifiNetworks");
    numberOfNetworks = WiFi.scanNetworks();
    Serial.println("Found " + String(numberOfNetworks) + " networks.");
    if(_availableNetworks)
    {
        delete[] _availableNetworks;
    }

    _availableNetworks = new WifiNetwork[numberOfNetworks];

    for (int i = 0; i < numberOfNetworks; i++)
    {
        Serial.println("Adding network " + WiFi.SSID(i));
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
    _server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {SimUServeWiFi::handleRootGet(request);});
    _server->on("/network/all", HTTP_GET,  [this](AsyncWebServerRequest *request) {SimUServeWiFi::handleRefreshNetworksGet(request);});
    _server->on("/network", HTTP_POST, [this](AsyncWebServerRequest *request) {SimUServeWiFi::handleSaveNetwork(request);});
    _server->onNotFound([this](AsyncWebServerRequest *request) {SimUServeWiFi::handleNotFound(request);});
    _server->begin();
}

void SimUServeWiFi::checkForWebRequests(void)
{
    MDNS.update();
}

void SimUServeWiFi::handleRootGet(AsyncWebServerRequest *request)
{
    Serial.println("SimUServeWiFi::handleRootGet");
    request->send(SPIFFS, "index.min.html", "text/html");
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
    request->send(200, "application/json", returnJson);
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
    message += request->url();
    message += "\nMethod: ";
    message += ( request->method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += request->args();
    message += "\n";

    for ( uint8_t i = 0; i < request->args(); i++ ) {
        message += " " + request->argName ( i ) + ": " + request->arg ( i ) + "\n";
    }

    auto* response = request->beginResponse(404, "text/plain", message);
    response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    response->addHeader("Pragma", "no-cache");
    response->addHeader("Expires", "-1");
    response->addHeader("Content-Length", String(message.length()));
    request->send(response);
}

SimUServeWiFi simUServeWiFi;