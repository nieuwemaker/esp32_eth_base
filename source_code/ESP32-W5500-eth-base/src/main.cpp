/*
 * Author: Renzo Mischianti
 * Website: mischianti.org
 * Description: This sketch demonstrates how to set up and use Ethernet on an ESP32 module using the W5500 Ethernet module.
 *              The sketch initializes the Ethernet module, handles Ethernet events, and tests a simple client connection.
 */
 
#include "Arduino.h"
 
#include <ETH.h>
#include <SPI.h>
 
// Set this to 1 to enable dual Ethernet support
#define USE_TWO_ETH_PORTS 0
 
// #ifndef ETH_PHY_TYPE
#define ETH_PHY_TYPE          ETH_PHY_W5500
#define ETH_PHY_ADDR          1
#define ETH_PHY_CS            5 // CS esp32s3  10  |  esp32   5
#define ETH_PHY_IRQ          -1 //              9  |         15
#define ETH_PHY_RST          -1 //              3  |          4
// #endif
 
// SPI pins                         esp32s3     esp32
#define ETH_SPI_SCK         SCK  // 12          18
#define ETH_SPI_MISO        MISO // 13          19
#define ETH_SPI_MOSI        MOSI // 11          23
 
static bool eth_connected = false;
 
// Ethernet event handler
void onEvent(arduino_event_id_t event, arduino_event_info_t info)
{
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      // Set Ethernet hostname here
      ETH.setHostname("esp32-eth0");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.printf("ETH Got IP: '%s'\n", esp_netif_get_desc(info.got_ip.esp_netif));
      Serial.println(ETH);
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_LOST_IP:
      Serial.println("ETH Lost IP");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}
 
// Test client connection
void testClient(const char * host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);
 
  NetworkClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.println("GET /get HTTP/1.1");
  client.println("Host: httpbin.org");
  client.println("Connection: close");
  client.println();
 
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }
 
  Serial.println("closing connection\n");
  client.stop();
}
 
void setup()
{
  Serial.begin(115200);
  Network.onEvent(onEvent);
 
  SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI, ETH_PHY_CS);
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_CS, ETH_PHY_IRQ, ETH_PHY_RST, SPI);
}
 
void loop()
{
  if (eth_connected) {
    testClient("httpbin.org", 80);
  }
  delay(10000);
}