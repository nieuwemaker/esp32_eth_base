#include <network/WifiNetwork.h>
//#include <network/HttpServer.h>
#include <network/ArtnetClient.h>
#include "PerformanceData.h"

#include "FastLED.h"

#define SINGLE_TIME_UNIT       5
#define SECOND_TIME_UNIT       200 // 200 * 5 = 1000 ms
#define FRAME_UPDATE_TIME_UNIT 4 // 50 fps for bandwidth //3 // every 15ms a redraw of the screen = 67 fps (4*5)
#define BRIGHTNESS_TIME_UNIT   50

/* Timing */
uint32_t shortCyleCounter    = 0; // Counter to know how many cycles have passed since last 5ms check window
uint32_t secondCycleCounter  = 0; // Counter to know how many cycles have passed since last second
uint32_t fpsCycleCounter     = 0; // Counter to know how many cycles have passed sincs last frame


WifiNetwork     *wifiNetwork;
//HttpServer      *httpServer;
PerformanceData *performance;
ArtnetClient    *artnetClient;

void setup() {
  //httpServer   = new HttpServer();
  wifiNetwork  = new WifiNetwork();
  artnetClient = new ArtnetClient();
  performance = PerformanceData::GetInstance();
  wifiNetwork->Connect();
  //httpServer->Start();
  artnetClient->Connect();
}

void loop() {
  EVERY_N_MILLISECONDS(SINGLE_TIME_UNIT){
    //httpServer->Listen();
  }
  EVERY_N_MILLISECONDS(SINGLE_TIME_UNIT * SECOND_TIME_UNIT){ performance->ResetCounters(); }
  performance->CountLoop();
}
