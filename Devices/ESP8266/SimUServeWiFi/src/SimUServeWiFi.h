#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include "SimUServeWiFiSettings.h"

#define MAX_SSID_LENGTH 32
#define MAX_PASSWORD_LENGTH 63
#define DEFAULT_SERVER_PORT 80
#define DEFAULT_SERVER_IP "10.0.1.1"
#define DEFAULT_SERVER_SSID "SimUServeWiFiHost"
#define DEFAULT_SERVER_PASSWORD "SimUServeWiFIPassword"

class SimUServeWiFi {
  protected:
    // contains the settings for both the server and the WiFiSettings
    // we want to connect to
    SimUServeWiFiSettings* _settings;
    MDNSResponder* _mdns ;
    WiFiServer* _server;
  
  private:
    const int _ssidStorageOffset = 0;
    const int _passwordStorageOffset = 33;

  public:
    SimUServeWiFi();
    SimUServeWiFi(int, String const&);
    ~SimUServeWiFi();
    String const& getWiFiSsid(void) const;
    String const& getWiFiPassword(void) const;
    void setWifiSsid(String const&);
    void setWifiPassword(String const&);
    
  protected:
    template <typename T>
    bool checkEepromForValue(int, T&);
    template <typename T>
    void writeValueToEeprom(int, T const&);
    // Inits some defaults
    void initDefaults(void);
    // Tests the WiFi connection to see if Wifi is working.  If not, returns false.
    bool testWifiConnection(void);

    void startMdns(void);
};

#endif
