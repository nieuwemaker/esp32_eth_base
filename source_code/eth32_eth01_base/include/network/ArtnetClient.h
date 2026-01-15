/*
 * WifiNetwork.h
 *
 *  Created on: January 10, 2025
 *      Author: wvanvliet
 * 
 *  For Ethernet shields using WT32_ETH01 (ESP32 + LAN8720)
 *
 */
#include <network/AbstractNetworkInterface.h>
//#include <EthernetESP32.h>
//#include <QNEthernet.h>
//#include "WebServer_WT32_ETH01.h"
//#include "WebServer_WT32_ETH01.hpp"  
#include <ETH.h>
#include <Udp.h>

#ifndef ARTNETCLIENT_H_
#define ARTNETCLIENT_H_

#define ETH_PHY_TYPE    ETH_PHY_LAN8720
#define ETH_ADDR        1
#define ETH_POWER_PIN   16
#define ETH_MDC_PIN     23
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_MDIO_PIN    18
#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT

#define HOST_NAME "pixelblok"

#define ARTNET_DMX_HEADER_SIZE         18
#define ARTNET_LISTENING_PORT          6454
#define ARTNET_MAX_PIXELS_PER_UNIVERSE 170
#define ARTNET_MAX_BUFFER              530
#define ARTNET_MANUFACTURER_ID         0x7FF1 // ESTA This a an ID intended for prototyping and not for commercial use (https://tsp.esta.org/tsp/working_groups/CP/mfctrIDs.php)
#define ARTNET_OEM_CODE                42 // Product code
#define ARTNET_SHORT_NAME              "NMPP"
#define ARTNET_LONG_NAME               "NieuweMaker Pixel Perfect"

class ArtnetClient : public AbstractNetworkInterface{
    public:
        ArtnetClient();
        bool 
            IsConnected(),
            Connect();
        void DisConnect(),
            Receive(),
             PrintStatus();
    protected:

    private:
        IPAddress staticIP     = IPAddress("10.0.0.2");
        IPAddress staticSubnet = IPAddress("255.0.0.0");
        uint8_t mac[6]         = {0xDE, 0xAD, 0xBE, 0xEE, 0xFE, 0xEE};
        bool udpStarted = false;
        NetworkUDP udpClient;
        //NetworkUDP udpClient;
        //char packetBuffer[255];
        //WiFiUDP udpClient;
};
#endif