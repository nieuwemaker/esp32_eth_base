#include <network/ArtnetClient.h>

ArtnetClient::ArtnetClient() : AbstractNetworkInterface(){
    WT32_ETH01_onEvent(); // needs to be called before connecting Ethernet client
}

bool ArtnetClient::IsConnected(){
    return false;
}
bool ArtnetClient::Connect(){
    /*
    if(!Ethernet.connected()){
        Ethernet.begin(this->staticIP,INADDR_NONE,INADDR_NONE,this->staticSubnet);
        this->udp = NetworkUDP();
        this->udp.begin(Ethernet.localIP(),ARTNET_LISTENING_PORT);
    */
    ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);
    ETH.config(this->staticIP, IPAddress("0.0.0.0"), this->staticSubnet, IPAddress("0.0.0.0"));
    //WT32_ETH01_waitForConnect();
    this->udpClient.begin(ARTNET_LISTENING_PORT);
    return true;
}
void ArtnetClient::DisConnect(){
    this->udpClient.flush();
    this->udpClient.stop();
    ETH.end();
}
void ArtnetClient::PrintStatus(){

}