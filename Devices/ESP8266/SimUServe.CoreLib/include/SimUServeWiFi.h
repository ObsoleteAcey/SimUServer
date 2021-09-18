/*
* Copyright (C) 2020 Stefan Park - All rights reserved
* This is not free software and is not open source.  This
* is part of the SimUServe ecosystem and is designed to
* run as part of such.  Please do not redistribute this source
* code in any form.  If you wish to license this software, please
* contact Stefan at stefan.john.park@gmail.com.
*/

#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include "SimUServeNetworkSettings.h"
#include "SimUServeCommon.h"
#include <stdint.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>


#define MAX_SSID_LENGTH 32
#define MAX_PASSWORD_LENGTH 63
#define DEFAULT_SERVER_PORT 80
#define DEFAULT_AP_IP "10.0.1.1"
#define DEFAULT_AP_GATEWAY "10.0.1.1"
#define DEFAULT_AP_NETMASK "255.255.255.0"
#define DEFAULT_AP_SSID "SimUServeWiFiHost_"
#define DEFAULT_AP_PASSWORD "SimUServeWiFIPassword"
#define SERVER_LOCAL_ADDRESS "http://simuserve.device"

struct WifiNetwork {
  int Index;
  String SSID;
  int32_t RSSI;
  int EncryptionType;

  WifiNetwork(){}

  WifiNetwork(int const index, String const& ssid, int32_t const rssi, int encryptionType)
  {
    Index = index;
    SSID = String(ssid);
    RSSI = rssi;
    EncryptionType = encryptionType;
  }

  int getSignalStrength() 
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
    jsonString.concat(WifiNetwork::getSignalStrength());
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

class SimUServeWiFi {
  protected:
    // contains the settings for both the server and the WiFiSettings
    // we want to connect to
    SimUServeNetworkSettings* _settings;
    AsyncWebServer* _server;
    WifiNetwork* _availableNetworks;
    int numberOfNetworks;
  
  private:
    const int _ssidStorageOffset = 0;
    const int _passwordStorageOffset = 33;
    String _pageHtml;
    String _formHtml;
    String _ssidSelectionHtml;
    const String index_html = "<!DOCTYPE HTML><html><head><style>.tab{overflow:hidden;border:1px solid #ccc;background-color:#f1f1f1;}.tab button{background-color:inherit;float:left;border:none;outline:none;cursor:pointer;padding:14px 16px;transition:0.3s;}.tab button:hover{background-color:#ddd;}.tab button.active{background-color:#ccc;}.tabcontent{display:none;padding:6px 12px;border:1px solid #ccc;border-top:none;}.error{border-color:red;}</style><title>SimUServe{{deviceName}} setup page</title></head><body><div class=\"tab\"><button class=\"tablinks\" onclick=\"openTab(event,'networkSetup')\">ConnectionSettings</button><button class=\"tablinks\" onclick=\"openTab(event,'deviceSetup')\">Device Settings</button></div><div id=\"networkSetup\" class=\"tabcontent\"><h1>Welcome to the SimUServe network setup</h1><form action=\"javascript:void(0);\"><div class=\"ssid-container\"><label for=\"ssid\">SSID:</label><select id=\"ssid\" name=\"ssid\" onchange=\"ssidSelected(event)\" ><option value=\"PleaseSelect\">Please select an SSID</option><option value=\"DeepSpaceNine\">DeepSpaceNine</option><option value=\"DeepSpaceNine_5g\">DeepSpaceNine_5g</option></select></div><div class=\"password-container\"><label for=\"password\">Password:</label><input id=\"password\" type=\"password\"><input type=\"checkbox\" onclick=\"togglePasswordVisibility()\">Show Password</div><div class=\"button-container\"><button class=\"save\" onclick=\"saveNetworkSettings()\">Save</button><button class=\"refesh-networks\" onclick=\"refeshNetworks()\">Refresh Networks</button></div></form></div><div id=\"deviceSetup\" class=\"tabcontent\"><h1>Welcome to the SimUServe device setup</h1></div><script>function removeClass(element,className){element.className=element.className.replace(className,"");}function addClass(element,className){element.className+=className;}function removeAllOptions(optionElementId){var select=document.getElementById(optionElementId);select.options.length=0;}function addOption(optionElementId,optionDisplay,optionValue,defaultSelected,selected){var select=document.getElementById(optionElementId);select.options[select.options.length]=new Option(optionDisplay,optionValue,defaultSelected,selected);}function togglePasswordVisibility(){var passwordText=document.getElementById('password');if(passwordText.type==='password'){passwordText.type='text';}else{passwordText.type='password';}}function openTab(evt,tabToOpen){var i,tabcontent,tablinks;tabcontent=document.getElementsByClassName('tabcontent');for(i=0;i<tabcontent.length; i++){tabcontent[i].style.display='none';}tablinks=document.getElementsByClassName('tablinks');for(i=0;i<tablinks.length; i++){removeClass(tablinks[i],' active');}document.getElementById(tabToOpen).style.display=\"block\";addClass(evt.currentTarget, ' active');}function ssidSelected(event){var ssidCombo=event.currentTarget;if(ssidCombo.value==='PleaseSelect'){addClass(ssidCombo,' error');return;}removeClass(ssidCombo,' error');}function saveNetworkSettings(){var ssidCombo=document.getElementById('ssid');if(ssidCombo.value==='PleaseSelect'){addClass(ssidCombo,' error');return;}}function refreshNetworks(){}</script></body></html>";

  public:
    SimUServeWiFi();
    /**
     * @brief Construct a new Sim U Serve Wi Fi object
     * @param SSID SSID of the WiFi network being connected to
     * @param port 
     */
    SimUServeWiFi(String const&, uint16_t);
    ~SimUServeWiFi();
    String const& getWiFiSsid(void) const;
    String const& getWiFiPassword(void) const;
    void setWifiSsid(String const&);
    void setWifiPassword(String const&);

    /*
      Call this every loop to check for any web requests and update MDNS
    */
    void checkForRequests(void);

    /*
      Call this during setup, after params have been set, to start MDNS and the WebServer
    */
    void initServices(void);

    /*
     * Tests the WiFi connection to see if Wifi is working.  If not, returns false.
    */
    bool testWifiConnection(void);
    
    String getWifiStatus(wl_status_t);

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
    WifiNetwork* const getAvailableWifiNetworks(void);

    /*
     * refreshes the list of available Wifi networks that can be connected to
    */
    WifiNetwork* const refreshAvailableWifiNetworks(void);

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
    * Processes the network save for the SSID
    */
    void handleSaveNetwork(AsyncWebServerRequest*);

    void handleNotFound(AsyncWebServerRequest*);
};

#endif
