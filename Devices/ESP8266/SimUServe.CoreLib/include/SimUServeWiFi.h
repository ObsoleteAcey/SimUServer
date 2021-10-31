/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#ifndef SIMUSERVEWIFI_H
#define SIMUSERVEWIFI_H

#include "SimUServeNetworkSettingsManager.h"
#include "SimUServeCommon.h"
#include <stdint.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>


#define MAX_SSID_LENGTH 32
#define MAX_PASSWORD_LENGTH 63


struct WiFiNetwork {
  int Index;
  String SSID;
  int32_t RSSI;
  int EncryptionType;

  WiFiNetwork(){}

  WiFiNetwork(int const index, String const& ssid, int32_t const rssi, int encryptionType)
  {
    Index = index;
    SSID = String(ssid);
    RSSI = rssi;
    EncryptionType = encryptionType;
  }

  int32_t getSignalStrength() 
  {
      if(RSSI <= -100)
      {
          return 0;
      }
      if(RSSI >= -50)
      {
          return 100;
      }
      return 2 * (RSSI + 100);
  }

  String const toJson(void) 
  {
    String jsonString = "{\"ssid\":\"";
    jsonString.concat(SSID);
    jsonString.concat("\",\"signalStrength\":\"");
    jsonString.concat(WiFiNetwork::getSignalStrength());
    jsonString.concat("\",\"index\":\"");
    jsonString.concat(Index);
    jsonString.concat("\",\"encryptionType\":\"");
    switch(EncryptionType)
    {
        case ENC_TYPE_NONE:
            jsonString.concat("None");
            break;
        case ENC_TYPE_WEP:
            jsonString.concat("WEP");
            break;
        case ENC_TYPE_TKIP:
            jsonString.concat("WPA-PSK");
            break;
        case ENC_TYPE_CCMP:
            jsonString.concat("WPA2-PSK");
            break;
        case ENC_TYPE_AUTO:
            jsonString.concat("WPA+WPA2-PSK");
            break;
        default:
            jsonString.concat("Unknown");
      }
      jsonString.concat("\"}");

      return jsonString;
  }
};

/**
 * @brief Used for connecting to WiFi
 * 
 */
class SimUServeWiFi {
  protected:
    // contains the settings for both the server and the WiFiSettings
    // we want to connect to
    SimUServeNetworkSettingsManager* _settingsManager;
    AsyncWebServer* _server;
    WiFiNetwork* _availableNetworks;
    int _numberOfNetworks;
  
  private:
    const int _ssidStorageOffset = 0;
    const int _passwordStorageOffset = 33;

  public:

    /**
     * @brief Construct a new Sim U Serve Wi Fi object with default settings
     * 
     */
    SimUServeWiFi();
    
    /**
     * @brief Construct a new Sim U Serve Wi Fi object
     * @param SSID SSID of the WiFi network being connected to
     * @param port 
     */
    SimUServeWiFi(String const&, uint16_t);


    ~SimUServeWiFi();

    /**
     * @brief Get the Wi Fi Ssid object
     * 
     * @return String const& 
     */
    String const& getWiFiSsid(void) const;

    /**
     * @brief Get the Wi Fi Password object
     * 
     * @return String The currently set WiFi  security key
     */
    String const& getWifiNetworkSecurityKey(void) const;

    /**
     * @brief Set the Wifi Ssid object
     * 
     */
    void setWifiSsid(String const&);

    /**
     * @brief Set the Wifi Password object
     * 
     */
    void setWifiNetworkSecurityKey(String const&);

    /**
     * @brief Call this every loop to check for any web requests and update MDNS
    */
    void checkForRequests(void);

    /**
     * @brief Call this during setup, after params have been set, to start MDNS and the SoftAP.
     * NOTE: can be handled automatically by calling initWifiServices()
    */
    void initAccessPointServices(void);

    /**
     * @brief Call this during setup, after params have been set, to start MDNS and the WebServer.
     * NOTE: can be handled automatically by calling initWifiServices()
    */
    void initConfigServices(void);

    /**
     * @brief Check if WiFi is connected.  Can be called at any time and simply looks at the WiFi status.
     * DOES NOT change Wifi mode.
     * 
     * @return true 
     * @return false 
     */
    bool checkWiFiIsConnected(void);

    /**
     * @brief Tests the WiFi connection to see if Wifi is working.  If not, returns false.
     * can be called after credentials are saved to attempt a connection to a WiFi station.
     * DOES NOT change WiFi mode.  Typically used when in config (SoftAP)
    */
    bool testWifiConnection(void);
    
    /**
     * @brief Get the Wifi Status as a string
     * 
     * @return String The status of the WiFi connection
     */
    static String getWifiStatus(void);

  protected:
    template <typename T>
    bool checkEepromForValue(int, T&);
    template <typename T>
    void writeValueToEeprom(int, T const&);
    
    /*
      Inits some default settings.  These will be over-ridden by any saved settings
    */
    void initDefaults(void);

    /*
      Fires up the MDNS Responder
    */
    void startMdns(void);

    /*
     * gets a list of available Wifi networks that can be connected to
    */
    WiFiNetwork* const getAvailableWifiNetworks(void);

    /*
     * refreshes the list of available Wifi networks that can be connected to
    */
    WiFiNetwork* const refreshAvailableWifiNetworks(void);

    String const networksToJson(void);
    
    /*
     * sets up the access point and populates available wireless networks
    */
    void setupAccessPoint(void);

    /*
    * Launch the webserver
    */
    void launchWebServer(void);
  
    /*
    * handles requests to the root page
    */
    void handleRootGet(AsyncWebServerRequest*);
    
    /*
    *  handles a request to refresh the available networks
    */
    void handleRefreshNetworksGet(AsyncWebServerRequest*);

    /*
    *  handles a request to return the available networks
    */
    void handleAllNetworksGet(AsyncWebServerRequest*);

    /*
    * Processes the network save for the SSID
    */
    void handleSaveNetwork(AsyncWebServerRequest*);

    void handleNotFound(AsyncWebServerRequest*);

    
};

#endif
