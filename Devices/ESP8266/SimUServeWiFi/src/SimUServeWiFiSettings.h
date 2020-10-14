#ifndef SimUServeWiFiSettings_h
#define SimUServeWiFiSettings_h

#include <ESP8266WiFi.h>

class SimUServeWiFiSettings {
  protected:
    int _serverPort;
    String* _serverIpAddress;
    String* _serverSsid;
    String* _serverPassword;
    String* _connectedSsid;
    String* _connectedPassword;
    String* _deviceHostName;

  private:
    bool _isDirty = false;
    IPAddress* _ipAddress;
    
  public:
    ~SimUServeWiFiSettings();
    void setServerPort(int);
    int getServerPort(void) const;
    void setServerIpAddress(String const&);
    IPAddress const& getServerIpAddress(void) const;
    void setServerSsid(String const&);
    String const& getServerSsid(void) const;
    void setServerPassword(String const&);
    String const& getServerPassword(void) const;
    void setConnectedSsid(String const&);
    String const& getConnectedSsid(void) const;
    void setConnectedPassword(String const&);
    String const& getConnectedPassword(void) const;
    void setDeviceHostName(String const&);
    String const& getDeviceHostName(void) const;
    void update(SimUServeWiFiSettings const&);
};

#endif
