#ifndef SimUServeWiFi_h
#define SimUServeWiFi_h

#include <ESP8266WiFi.h>
#include <EEPROM.h>

class SimUServeWiFi {
  protected:
    int _serverPort = 80;
    String _serverIpAddress = String("192.168.1.100");
    String _wifiSsid;
    String _wifiPassword;
  
  private:
    const int _ssidStorageOffset = 0;
    // 
    const int _maxSsidLength = 32;
    int _passwordStorageOffset = 33;

  public:
    SimUServeWiFi(int, String);
    String getWiFiSsid();
    String getWiFiPassword();
    
  protected:
    bool checkEepromForValue(int, int, String&);
      
};

#endif
