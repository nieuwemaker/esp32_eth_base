
//#define FASTLED_USES_ESP32S3_I2S 
#define FASTLED_USES_ESP32S3_SPI 

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <FastLED.h>

/**
 * SYSTEM DEFINES
 */
#define CPU_SPEED 240000000
#define SPI_SPEED  80000000 // The MAX W5500’s SPI speed according to documentation
/**
 * ETHERNET DEFINES
 */
#define ETH_PHY_CS           SS 
// SPI pins                      
#define ETH_SPI_SCK         SCK 
#define ETH_SPI_MISO        MISO
#define ETH_SPI_MOSI        MOSI

/**
 * ARTNET DEFINES
 */
#define OPPOLL        0x2000 // This is an ArtPoll packet, no other data is contained in this UDP packet.
#define OPPOLLREPLY   0x2100 // This is an ArtPollReply Packet. It contains device status information.
#define OPDIAGDATA    0x2300 // Diagnostics and data logging packet.
#define OPCOMMAND     0x2400 // This is an ArtCommand packet. It is used to send text based parameter commands.
#define OPDATAREQUEST 0x2700 // This is an ArtDataRequest packet. It is used to request data such as products URLs
#define OPDATAREPLY   0x2800 // This is an ArtDataReply packet. It is used to reply to ArtDataRequest packets.
#define OPDMX         0x5000 // This is an ArtDmx data packet. It contains zero start code DMX512 information for a single Universe.
#define OPNZS         0x5100 // This is an ArtNzs data packet. It contains non-zero start code (except RDM) DMX512 information for a single Universe.
#define OPSYNC        0x5200 // This is an ArtSync data packet. It is used to force synchronous transfer of ArtDmx packets to a node’s output.
#define OPADDRESS     0x6000 // This is an ArtAddress packet. It contains remote programming information for a Node.
#define OPINPUT       0x7000 // This is an ArtInput packet. It contains enable – disable data for DMX inputs.
#define OPTODREQUEST  0x8000 // This is an ArtTodRequest packet. It is used to request a Table of Devices (ToD) for RDM discovery.
#define OPTODDATA     0x8100 // This is an ArtTodData packet. It is used to send a Table of Devices (ToD) for RDM discovery

#define ARTNET_DMX_HEADER_SIZE         18
#define ARTNET_LISTENING_PORT          6454
#define ARTNET_MAX_PIXELS_PER_UNIVERSE 170
#define ARTNET_MAX_BUFFER              530
#define ARTNET_MANUFACTURER_ID         0x7FF1 // ESTA This a an ID intended for prototyping and not for commercial use (https://tsp.esta.org/tsp/working_groups/CP/mfctrIDs.php)
#define ARTNET_OEM_CODE                42 // Product code
#define ARTNET_SHORT_NAME              "NM_PIXEL_CONTROL"
#define ARTNET_LONG_NAME               "NieuweMaker Pixel Controller"

/**
 * LED DEFINES
 */
#define MAX_LEDS_PER_PIN     512
#define LED_PIN_COUNT         10
#define PIXEL_MINI_LED_COUNT 256
#define PIXEL_BAR_LED_COUNT   64

#define INTERNAL_LED 48
#define LED_PIN_0  1
#define LED_PIN_1  2
#define LED_PIN_2  42
#define LED_PIN_3  41
#define LED_PIN_4  40
#define LED_PIN_5  39
#define LED_PIN_6  38
#define LED_PIN_7  37
#define LED_PIN_8  36
#define LED_PIN_9  35

// From: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
uint8_t ledPins[LED_PIN_COUNT] = {LED_PIN_0, LED_PIN_1,LED_PIN_2,LED_PIN_3,LED_PIN_4,LED_PIN_5,LED_PIN_6,LED_PIN_7,LED_PIN_8,LED_PIN_9};
CRGB    leds[LED_PIN_COUNT][MAX_LEDS_PER_PIN];
CRGB    intLed[1];
bool    updateLeds = false;

// Base code for artnetreply from: https://github.com/natcl/Artnet/blob/master/Artnet.h
struct artnet_poll_reply {
    uint8_t  id[8];
    uint16_t opCode;
    uint8_t  ip[4];
    uint16_t port;
    uint8_t  verH;
    uint8_t  ver;
    uint8_t  subH;
    uint8_t  sub;
    uint8_t  oemH;
    uint8_t  oem;
    uint8_t  ubea;
    uint8_t  status;
    uint8_t  etsaman[2];
    uint8_t  shortname[18];
    uint8_t  longname[64];
    uint8_t  nodereport[64];
    uint8_t  numbportsH;
    uint8_t  numbports;
    uint8_t  porttypes[4];//max of 4 ports per node
    uint8_t  goodinput[4];
    uint8_t  goodoutput[4];
    uint8_t  swin[4];
    uint8_t  swout[4];
    uint8_t  swvideo;
    uint8_t  swmacro;
    uint8_t  swremote;
    uint8_t  sp1;
    uint8_t  sp2;
    uint8_t  sp3;
    uint8_t  style;
    uint8_t  mac[6];
    uint8_t  bindip[4];
    uint8_t  bindindex;
    uint8_t  status2;
    uint8_t  filler[26];
  } __attribute__((packed));
struct artnet_poll_reply  artPollReply;

// put function declarations here:
void loopCore0(void* pvParameters);
bool InitializeEthernet();
void monitorEthernetConnection();
bool sendOPPOLREPLY();
bool handleOPDMXPacket(char udpData[]);
bool isConnected();
void tryUpdateLeds();

IPAddress   connectedIP;
IPAddress   localIP;
bool        connected;
EthernetUDP udp;
bool opSync = false;

int Core0PerformanceCounter = 0;
int Core1PerformanceCounter = 0;
int udpCounter              = 0;
int fpsCounter              = 0;
///////////////////////////////////////////////////
// ETHERNET FUNCTIONS
///////////////////////////////////////////////////
bool InitializeEthernet(){
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  Ethernet.init(ETH_PHY_CS);
  if(Ethernet.begin(mac)){
    Serial.printf("DHCP OK! --> Awaiting IP data\n");
    uint8_t timeOut = 0;
    while(!isConnected() && timeOut < 12){
      delay(500);
      Serial.printf("Waiting: %d\n",timeOut/2.0f);
    }
    Serial.print("Local IP : ");
    Serial.println(Ethernet.localIP());
    Serial.print("Subnet Mask : ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("Gateway IP : ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("DNS Server : ");
    Serial.println(Ethernet.dnsServerIP());
    // Connect other services
    localIP = Ethernet.localIP();
    udp.begin(ARTNET_LISTENING_PORT);
    Serial.println("UDP started");
    return true;
  }
  return false;
}

bool isConnected(){
  return Ethernet.linkStatus() == 1;
}

void monitorEthernetConnection(){
  Serial.printf("Link: %d, Hardware: %d\n", Ethernet.linkStatus(), Ethernet.hardwareStatus());
}

///////////////////////////////////////////////////
// ARTNET FUNCTIONS
///////////////////////////////////////////////////
bool ReceiveData(){
  if(!connected){ return false; }
  int packetSize = udp.parsePacket();
  if(packetSize < 1){ return false; } //test for empty packet
  char udpData[packetSize];
  connectedIP = udp.remoteIP();
  udp.read(udpData, packetSize);
  uint16_t opCode = ((udpData[9] << 8) | udpData[8]);
  if(opCode == OPDMX){
      return handleOPDMXPacket(udpData);
  } else if(opCode == OPPOLL){
    return sendOPPOLREPLY();
    return true;
  } else if(opCode == OPSYNC){ // Syncing refresh on packets
    opSync = true;
    tryUpdateLeds();
    return true;
  } else {
      Serial.printf("Other OPCODE found %x \n", opCode);
  }
  return false;
}

bool handleOPDMXPacket(char udpData[]){
  udpCounter++;
  uint16_t universe = ((udpData[15] << 8) | udpData[14]);
  int pcnt          = ARTNET_DMX_HEADER_SIZE;
  for(int n=0;n<ARTNET_MAX_PIXELS_PER_UNIVERSE;n++){
      uint16_t totalIndex = (universe * ARTNET_MAX_PIXELS_PER_UNIVERSE) + n;
      uint8_t  pinIndex   = totalIndex / MAX_LEDS_PER_PIN;//PIXEL_MINI_LED_COUNT;//MAX_LEDS_PER_PIN;
      uint16_t ledIndex   = totalIndex % MAX_LEDS_PER_PIN;//PIXEL_MINI_LED_COUNT;//MAX_LEDS_PER_PIN; 
      leds[pinIndex][ledIndex] = CRGB(udpData[pcnt++], udpData[pcnt++], udpData[pcnt++]);
  }
  updateLeds = true;
  return true;
}

bool sendOPPOLREPLY(){
    
    uint8_t  node_ip_address[4];
    uint8_t  id[8];
    node_ip_address[0] = Ethernet.localIP()[0];
    node_ip_address[1] = Ethernet.localIP()[1];
    node_ip_address[2] = Ethernet.localIP()[2];
    node_ip_address[3] = Ethernet.localIP()[3];

    sprintf((char *)id, "Art-Net");
    memcpy(artPollReply.id, id,              sizeof(artPollReply.id));
    memcpy(artPollReply.ip, node_ip_address, sizeof(artPollReply.ip));
    artPollReply.opCode = OPPOLLREPLY;
    artPollReply.port   = ARTNET_LISTENING_PORT;
    memset(artPollReply.goodinput,  0x08, 4);
    memset(artPollReply.goodoutput, 0x80, 4);
    memset(artPollReply.porttypes,  0xc0, 4);

    uint8_t shortname[18];
    uint8_t longname [64];
    sprintf((char *)shortname, ARTNET_SHORT_NAME);
    sprintf((char *)longname,  ARTNET_LONG_NAME );
    memcpy(artPollReply.shortname, shortname, sizeof(shortname));
    memcpy(artPollReply.longname,  longname,  sizeof(longname));

    artPollReply.etsaman[0] = ARTNET_MANUFACTURER_ID;       // Low byte
    artPollReply.etsaman[1] = ARTNET_MANUFACTURER_ID >> 8;  // High byte
    artPollReply.verH       = 1;
    artPollReply.ver        = 0;
    artPollReply.subH       = 0;
    artPollReply.sub        = 0;
    artPollReply.oemH       = 0;
    artPollReply.oem        = 0xFF;
    artPollReply.ubea       = 0;
    artPollReply.status     = 0xd2;
    artPollReply.swvideo    = 0;
    artPollReply.swmacro    = 0;
    artPollReply.swremote   = 0;
    artPollReply.style      = 0x06;

    artPollReply.numbportsH = 0;
    artPollReply.numbports  = 4;
    artPollReply.status2    = 0x08;
  
    artPollReply.bindip[0] = node_ip_address[0];
    artPollReply.bindip[1] = node_ip_address[1];
    artPollReply.bindip[2] = node_ip_address[2];
    artPollReply.bindip[3] = node_ip_address[3];

    uint8_t swin[4]  = {0x01,0x02,0x03,0x04};
    uint8_t swout[4] = {0x01,0x02,0x03,0x04};
    for(uint8_t i = 0; i < 4; i++)
    {
        artPollReply.swout[i] = swout[i];
        artPollReply.swin[i]  = swin[i];
    }
    sprintf((char *)artPollReply.nodereport, "%i DMX output universes active.", artPollReply.numbports);
    
    udp.beginPacket(connectedIP,ARTNET_LISTENING_PORT);
    udp.write((uint8_t *)&artPollReply, sizeof(artPollReply));
    udp.endPacket();
    return true;
}

///////////////////////////////////////////////////
// LED FUNCTIONS
///////////////////////////////////////////////////

void InitializeFastLed(){
  FastLED.addLeds<WS2812B, INTERNAL_LED, GRB>(intLed, 1).setCorrection(TypicalLEDStrip); // Internal for diagnosing
  FastLED.addLeds<WS2812B, LED_PIN_0, GRB>(leds[0], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds[1], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds[2], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds[3], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_4, GRB>(leds[4], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_5, GRB>(leds[5], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_6, GRB>(leds[6], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LED_PIN_7, GRB>(leds[7], MAX_LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(12);
}

void tryUpdateLeds(){
  fpsCounter++;
  if(updateLeds){
    FastLED.show();
    updateLeds = false;
  }
}

void setup() {
  InitializeFastLed();
  intLed[0] = CRGB::Red1;
  FastLED.show();
  Serial.begin(115200, SERIAL_8N1);
  Serial.end();
  Serial.begin(115200, SERIAL_8N1);
  Serial.println("Serial started");
  intLed[0] = CRGB::Orange1;
  FastLED.show();
  delay(200);
  //SPI.setFrequency(SPI_SPEED);
  SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI, ETH_PHY_CS);
  connected = InitializeEthernet();
  
  xTaskCreatePinnedToCore (
    loopCore0,     // Function to implement the task
    "loopCore0",   // Name of the task
    100000,      // Stack size in bytes
    NULL,      // Task input parameter
    10,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );
  intLed[0] = CRGB::Yellow1;
  FastLED.show();
  delay(500);
  intLed[0] = CRGB::Green1;
  FastLED.show();
}

void loop() {
  Core1PerformanceCounter++;
  EVERY_N_SECONDS(1){
    Serial.printf("Core 0: %d \tCore 1: %d \tFPS: %d \tART-NET: %d \tFREE HEAP: %d\n",Core0PerformanceCounter,Core1PerformanceCounter, fpsCounter, udpCounter, ESP.getFreeHeap());
    Core0PerformanceCounter = 0;
    Core1PerformanceCounter = 0;
    udpCounter              = 0;
    fpsCounter              = 0;
    intLed[0]  = (connected)? CRGB::Green1:CRGB::Blue1;
    updateLeds = true;
  }
}

void loopCore0(void* pvParameters){
  while(1){
    Core0PerformanceCounter++;
    ReceiveData();
    EVERY_N_MILLIS(20){
      if(!opSync){
        tryUpdateLeds();
      }
    }
  }
}