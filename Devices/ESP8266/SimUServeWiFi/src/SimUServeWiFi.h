#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define MAX_SSID_LENGTH 32;
#define MAX_PASSWORD_LENGTH 63;
#define DEFAULT_SERVER_PORT 80;
#define DEFAULT_SERVER_IP "10.0.1.1";
#define DEFAULT_SERVER_SSID "SimUServeWiFiHost";
#define DEFAULT_SERVER_PASSWORD "SimUServeWiFIPassword";

typedef struct WiFiSettings {
  int serverPort;
  String serverIpAddress;
  String serverSsid;
  String serverPassword;
  String connectedSsid;
  String connectedPassword;
};

class SimUServeWiFi {
  protected:
    // contains the settings for both the server and the WiFiSettings
    // we want to connect to
    WiFiSettings _settings;
  
  private:
    const int _ssidStorageOffset = 0;
    const int _passwordStorageOffset = 33;

  public:
    SimUServeWiFi();
    SimUServeWiFi(int, String);
    String getWiFiSsid();
    String getWiFiPassword();
    void setWifiSsid(String);
    void setWifiPassword(String);
    
  protected:
    bool checkEepromForValue(int, int, String&);
    void writeValueToEeprom(int, int, String);
    void initDefaults();
      
};

#endif
