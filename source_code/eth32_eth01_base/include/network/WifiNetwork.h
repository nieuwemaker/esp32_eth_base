/*
 * WifiNetwork.h
 *
 *  Created on: January 10, 2025
 *      Author: wvanvliet
 *  Goal:
 *  - Create possibility to do settings by creating WifiAP that you can connect to. You can:
 *      - Set wired Wifi setting
 *      - Set Wireless Wifi settings
 *  - Connect to wireless network for presenting web interface and artnet input
 * 
 *  Process:
 *  1. Check if ssid has value --> try connect
 *  2. id no ssid or connection fails --> setup own AP
 */
#include <network/AbstractNetworkInterface.h>
#include <WiFi.h>
#include <WiFiAP.h>

#ifndef WIFINETWORK_H_
#define WIFINETWORK_H_

#define WIFI_TIMEOUT_WINDOW 10'000
#define CONNECT_STATUS_NONE 0
#define CONNECT_STATUS_AP   1
#define CONNECT_STATUS_WIFI 2

class WifiNetwork : public AbstractNetworkInterface{
    public:
        WifiNetwork();
        bool 
            IsConnected(),
            Connect();
        void DisConnect(),
             PrintStatus();
        IPAddress GetIP();
    protected:
        //IPAddress MyIP;
    private:
        bool connectWithAP();
        bool connectWithWifi();
        const char *apSsid     = "PixelAP"; // AP name for setting up own Network
        char *ssid             = NULL; // Name of network to join
        char *password         = NULL; // Password of network to join
        uint8_t connStatus     = CONNECT_STATUS_NONE;
};
#endif