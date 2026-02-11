/*
 * Author: Renzo Mischianti
 * Website: mischianti.org
 * Description: This sketch demonstrates how to set up and use Ethernet on an ESP32 module using the W5500 Ethernet module.
 *              The sketch initializes the Ethernet module, handles Ethernet events, and tests a simple client connection.
 */
 
#include "Arduino.h"
 
#include <ETH.h>
#include <SPI.h>
#include <FastLED.h>

// Backup old SPI lines, but programming did not work
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
#define ETH_PHY_CS           HSPI_CS  
#define ETH_PHY_IRQ          HSPI_IRQ 
#define ETH_PHY_RST          HSPI_RST 
// #endif
 
// SPI pins                      
#define ETH_SPI_SCK         HSPI_SCK  
#define ETH_SPI_MISO        HSPI_MISO 
#define ETH_SPI_MOSI        HSPI_MOSI 
 
static bool eth_connected = false;

// From: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
uint8_t ledPins[12] = {5,16,17,18,19,21,23,25,26,27,32,33};
CRGB    leds[1024];

int performanceCounter = 0;
 
///////////////////////////////////////////////////
// ETHERNET FUNCTIONS
///////////////////////////////////////////////////
// Ethernet event handler
void onEvent(arduino_event_id_t event, arduino_event_info_t info)
{
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      // Set Ethernet hostname here
      ETH.setHostname("Pixel-block");
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

///////////////////////////////////////////////////
// LED FUNCTIONS
///////////////////////////////////////////////////

void initiateFastLed(){
  // {5,16,17,18,19,21,23,25,26,27,32,33};
  FastLED.addLeds<WS2812B, 5>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 16>(leds, 1024).setCorrection(TypicalLEDStrip);
  /*FastLED.addLeds<WS2812B, 17>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 18>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 19>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 21>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 23>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 25>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 26>(leds, 1024).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 27>(leds, 1024).setCorrection(TypicalLEDStrip);*/
  FastLED.setBrightness(20);
}

///////////////////////////////////////////////////
// SETUP && LOOP
///////////////////////////////////////////////////
 
void setup()
{

  Serial.begin(9600);
  Serial.println("Serial started");
  Network.onEvent(onEvent);

  initiateFastLed();
 
  SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI, ETH_PHY_CS);
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_CS, ETH_PHY_IRQ, ETH_PHY_RST, SPI);
}
 
void loop()
{
  performanceCounter++;
  EVERY_N_MILLISECONDS(10){
    FastLED.show();
  }
  EVERY_N_MILLISECONDS(1000){
    Serial.printf("Performance: %d\n",performanceCounter);
    performanceCounter = 0;
  }
}