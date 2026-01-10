#include <network/WifiNetwork.h>
#include <network/HttpServer.h>

WifiNetwork *wifiNetwork;
HttpServer  *httpServer;

void setup() {
  httpServer  = new HttpServer();
  wifiNetwork = new WifiNetwork();
  wifiNetwork->Connect();
  httpServer->Start();
}

void loop() {
  httpServer->Listen();
}
