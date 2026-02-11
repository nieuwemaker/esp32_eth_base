/*
 * Author: Renzo Mischianti
 * Website: mischianti.org
 * Description: This sketch demonstrates how to set up and use Ethernet on an ESP32 module using the W5500 Ethernet module.
 *              The sketch initializes the Ethernet module, handles Ethernet events, and tests a simple client connection.
 */
 
#include "Arduino.h"
 
#include <ETH.h>
#include <SPI.h>

#define DEFAULT_SCK  19
#define DEFAULT_MISO 18
#define DEFAULT_MOSI 23
#define DEFAULT_CS    2
#define DEFAULT_IRQ  22
#define DEFAULT_RST  15

#define HSPI_SCK  12
#define HSPI_MISO 14
#define HSPI_MOSI 13
#define HSPI_CS   15
#define HSPI_IRQ  22
#define HSPI_RST   4
 
// Set this to 1 to enable dual Ethernet support
#define USE_TWO_ETH_PORTS 0
 
// #ifndef ETH_PHY_TYPE
#define ETH_PHY_TYPE         ETH_PHY_W5500
#define ETH_PHY_ADDR         1
#define ETH_PHY_CS           HSPI_CS  //2 //5 // CS esp32s3    10  esp32   5
#define ETH_PHY_IRQ          HSPI_IRQ //22 //              9           15
#define ETH_PHY_RST          HSPI_RST //              3           4
// #endif
 
// SPI pins                         esp32s3     esp32
#define ETH_SPI_SCK         HSPI_SCK  //19 //SCK  // 12          18
#define ETH_SPI_MISO        HSPI_MISO //MISO // 13          19
#define ETH_SPI_MOSI        HSPI_MOSI //MOSI // 11          23
 
static bool eth_connected = false;

// From: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
uint8_t ledPins[12] = {5,16,17,18,19,21,23,25,26,27,32,33};
 
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

  Serial.begin(9600);
  Serial.println("Serial started");
  Network.onEvent(onEvent);
 
  SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI, ETH_PHY_CS);
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_CS, ETH_PHY_IRQ, ETH_PHY_RST, SPI);
  ETH.setHostname()
}
 
void loop()
{
  if (eth_connected) {
    //testClient("httpbin.org", 80);
  }
  delay(3000);
}