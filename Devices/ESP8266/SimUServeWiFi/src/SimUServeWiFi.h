#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define MAX_SSID_LENGTH 32;
#define MAX_PASSWORD_LENGTH 63;

class SimUServeWiFi {
  protected:
    // Port the Config server will listen on
    int _serverPort = 80;
    // Default IP the server will be at
    String _serverIpAddress = String("10.0.1.1");
    // The SSID for the WiFi network we want to connect to
    String _wifiSsid;
    // The Wifi password for the Wifo network we want to connect to
    String _wifiPassword;
  
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
      
};

#endif
