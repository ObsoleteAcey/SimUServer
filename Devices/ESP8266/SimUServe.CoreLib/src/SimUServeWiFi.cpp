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
    this->_settings = nullptr;
    this->_server = nullptr;
    this->_availableNetworks = nullptr;
    this->initDefaults();
    // now check and load any settings that have been saved
}

SimUServeWiFi::SimUServeWiFi(String const& serverSsid, uint16_t serverPort)
{
    Serial.println("SimUServeWiFi::SimUServeWiFi(" + String(serverPort) +", " + serverSsid + ")");
    this->_settings = nullptr;
    this->_server = nullptr;
    this->_availableNetworks = nullptr;
    this->initDefaults();
    this->_settings->setServerPort(serverPort);
    //_settings->setServerSsid(serverSsid);
}

SimUServeWiFi::~SimUServeWiFi()
{
    delete this->_server;
    delete[] this->_availableNetworks;
}

String const& SimUServeWiFi::getWiFiSsid(void) const
{
    return this->_settings->getConnectedNetworkSsid();
}

String const& SimUServeWiFi::getWiFiPassword(void) const
{
    return this->_settings->getConnectedNetworkSecurityKey();
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
    this->setupAccessPoint();
    this->startMdns();
}

void SimUServeWiFi::initDefaults()
{
    Serial.println("SimUServeWiFi::initDefaults");
    if(this->_settings == nullptr) 
    {
        this->_settings = SimUServeNetworkSettings::getSettings();;
    }

   this->_settings->setDeviceApIpAddress(DEFAULT_AP_IP);
   this->_settings->setDeviceApGatwayIpAddress(DEFAULT_AP_GATEWAY);
   this->_settings->setDeviceApNetmask(DEFAULT_AP_NETMASK);
   this->_settings->setDeviceApSsid(DEFAULT_AP_SSID + String(ESP.getChipId()));
   this->_settings->setDeviceApNetworkSecurityKey(DEFAULT_AP_PASSWORD + String(ESP.getChipId()));
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
        Serial.print("WiFi Status: " + this->getWifiStatus(WiFi.status()));    
        waitRetryCounter++;
    }
    Serial.println("Connect timed out");
    return(false);
}

void SimUServeWiFi::startMdns(void)
{
    Serial.println("SimUServeWiFi::startMdns");

    if(!MDNS.begin(SERVER_LOCAL_ADDRESS, this->_settings->getServerIpAddress())) {

    };
    this->launchWebServer();
    Serial.println("Adding http service to MDNS on port " + String(this->_settings->getServerPort()));
    MDNS.addService("http", "tcp", this->_settings->getServerPort());
}

void SimUServeWiFi::setupAccessPoint(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    this->getAvailableWifiNetworks();
    Serial.println("Starting access point on" + this->_settings->getDeviceApSsid() + " using password " + this->_settings->getDeviceApNetworkSecurityKey());
    WiFi.softAPConfig(this->_settings->getDeviceApIpAddress(), this->_settings->getDeviceApGatwayIpAddress(),
        _settings->getDeviceApNetmask());
    WiFi.softAP(this->_settings->getDeviceApSsid(), this->_settings->getDeviceApNetworkSecurityKey());
}

WifiNetwork* const SimUServeWiFi::getAvailableWifiNetworks(void)
{
    if(this->_availableNetworks)
    {
       return this->_availableNetworks;
    }

    Serial.println("SimUServeWiFi::getAvailableWifiNetworks");
    numberOfNetworks = WiFi.scanNetworks(false, true);
    Serial.println("Found " + String(numberOfNetworks) + " networks.");

    this->_availableNetworks = new WifiNetwork[numberOfNetworks];

    for (int i = 0; i < numberOfNetworks; i++)
    {
        Serial.println("Adding network " + WiFi.SSID(i));
        this->_availableNetworks[i] = WifiNetwork(i, WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
    }

    return this->_availableNetworks;
}

WifiNetwork* const SimUServeWiFi::refreshAvailableWifiNetworks(void)
{
    if(_availableNetworks)
    {
       delete[] _availableNetworks;
    }

    return this->getAvailableWifiNetworks();
}

void SimUServeWiFi::launchWebServer(void)
{
    Serial.println("SimUServeWiFi::launchWebServer");
    if(!this->_server)
    {
        Serial.println("SimUServeWiFi::launchWebServer");
        this->_server = new AsyncWebServer(_settings->getServerPort());
    }
    // set up the routes
    this->_server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {SimUServeWiFi::handleRootGet(request);});
    this->_server->on("/network/all", HTTP_GET,  [this](AsyncWebServerRequest *request) {SimUServeWiFi::handleRefreshNetworksGet(request);});
    this->_server->on("/network", HTTP_POST, [this](AsyncWebServerRequest *request) {SimUServeWiFi::handleSaveNetwork(request);});
    this->_server->onNotFound([this](AsyncWebServerRequest *request) {SimUServeWiFi::handleNotFound(request);});
    this->_server->begin();
}

void SimUServeWiFi::checkForRequests(void)
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
    auto* availableNetworks = this->getAvailableWifiNetworks();
    String returnJson = "{[";

    for (int i = 0; i < numberOfNetworks; i++)
    {
        String json = availableNetworks[i].toJson();
        returnJson.concat(json);
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
    if(this->testWifiConnection()) {
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

    for ( uint8_t i = 0; i < request->args(); i++ ) 
    {
        message += " " + request->argName ( i ) + ": " + request->arg ( i ) + "\n";
    }

    auto* response = request->beginResponse(404, "text/plain", message);
    response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    response->addHeader("Pragma", "no-cache");
    response->addHeader("Expires", "-1");
    response->addHeader("Content-Length", String(message.length()));
    request->send(response);
}

String SimUServeWiFi::getWifiStatus(wl_status_t status)
{
    switch(status){
        case WL_IDLE_STATUS:
            return "Idle";
            break;
        case WL_NO_SSID_AVAIL:
            return "No SSID available";
            break;
        case WL_SCAN_COMPLETED:
            return "Scan completed";
            break;
        case WL_CONNECTED:
            return "Connected";
            break;
        case WL_CONNECT_FAILED:
            return "Connection failed";
            break;
        case WL_CONNECTION_LOST:
            return "Connection lost";
            break;
        case WL_WRONG_PASSWORD:
            return "Wrong password";
            break;
        case WL_DISCONNECTED:
            return "Disconnected";
            break;
        default:
            return "Unknown error";
    }
}

