#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "SimUServeWiFiSettings.h"

#define MAX_SSID_LENGTH 32
#define MAX_PASSWORD_LENGTH 63
#define DEFAULT_SERVER_PORT 80
#define DEFAULT_SERVER_IP "10.0.1.1"
#define DEFAULT_SERVER_SSID "SimUServeWiFiHost"
#define DEFAULT_SERVER_PASSWORD "SimUServeWiFIPassword"
#define SERVER_LOCAL_ADDRESS "http://simuserve.device"

typedef struct WifiNetwork {
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
};

typedef WifiNetwork* WifiNetworkPtr;

class SimUServeWiFi {
  protected:
    // contains the settings for both the server and the WiFiSettings
    // we want to connect to
    SimUServeWiFiSettings* _settings;
    MDNSResponder* _mdns ;
    ESP8266WebServer* _server;
    WifiNetwork* _availableNetworks;
    int numberOfNetworks;
  
  private:
    const int _ssidStorageOffset = 0;
    const int _passwordStorageOffset = 33;
    String _pageHtml;
    String _formHtml;
    String _ssidSelectionHtml;

  public:
    SimUServeWiFi();
    SimUServeWiFi(int, String const&);
    ~SimUServeWiFi();
    String const& getWiFiSsid(void) const;
    String const& getWiFiPassword(void) const;
    void setWifiSsid(String const&);
    void setWifiPassword(String const&);
    void checkForWebRequests(void);
    
    
  protected:
    template <typename T>
    bool checkEepromForValue(int, T&);
    template <typename T>
    void writeValueToEeprom(int, T const&);
    // Inits some defaults
    void initDefaults(void);
    /*
     * Tests the WiFi connection to see if Wifi is working.  If not, returns false.
    */
    bool testWifiConnection(void);

    void startMdns(void);

    /*
     * gets a list of available Wifi networks that can be connected to
    */
    WifiNetwork* const getAvailableWifiNetworks(void);

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
    void handleRootGet(void);
    
    void handleRefreshNetworksGet(void);
};

#endif
