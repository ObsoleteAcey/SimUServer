#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#define MAX_SSID_LENGTH 32;
#define MAX_PASSWORD_LENGTH 63;
#define DEFAULT_SERVER_PORT 80;
#define DEFAULT_SERVER_IP "10.0.1.1";
#define DEFAULT_SERVER_SSID "SimUServeWiFiHost";
#define DEFAULT_SERVER_PASSWORD "SimUServeWiFIPassword";

class SimUServeWiFiSettings {
  protected:
    int _serverPort;
    String* _serverIpAddress;
    String* _serverSsid;
    String* _serverPassword;
    String* _connectedSsid;
    String* _connectedPassword;

  private:
    bool _isDirty = false;
    
  public:
    ~SimUServeWiFiSettings();
    void setServerPort(int);
    int getServerPort();
    void setServerIpAddress(const String&);
    IPAddress getServerIpAddress();
    void setServerSsid(const String&);
    String const& getServerSsid();
    void setServerPassword(const String&);
    String const& getServerPassword();
    void setConnectedSsid(const String&);
    String const& getConnectedSsid();
    void setConnectedPassword(const String&);
    String const& getConnectedPassword();
    void update(const SimUServeWiFiSettings&);
};

class SimUServeWiFi {
  protected:
    // contains the settings for both the server and the WiFiSettings
    // we want to connect to
    SimUServeWiFiSettings* _settings;

    MDNSResponder* mdns;
    WiFiServer* _server;
  
  private:
    const int _ssidStorageOffset = 0;
    const int _passwordStorageOffset = 33;

  public:
    SimUServeWiFi();
    SimUServeWiFi(int, String);
    ~SimUServeWiFi();
    String getWiFiSsid();
    String getWiFiPassword();
    void setWifiSsid(String);
    void setWifiPassword(String);
    
  protected:
    template <typename T>
    bool checkEepromForValue(int, T&);
    template <typename T>
    void writeValueToEeprom(int, T);
    // Inits some defaults
    void initDefaults();
    // Tests the WiFi connection to see if Wifi is working.  If not, returns false.
    bool testWifiConnection();

};

#endif
