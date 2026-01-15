#include <network/ArtnetClient.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

ArtnetClient::ArtnetClient() : AbstractNetworkInterface(){
    //EMACDriver driver(ETH_PHY_TYPE, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_POWER_PIN, EMAC_CLK_OUT_180_GPIO ,EMAC_CLK_OUT);
    //Ethernet.init(driver);
    //WT32_ETH01_onEvent(); // needs to be called before connecting Ethernet client
}

bool ArtnetClient::IsConnected(){
    return false;
}
bool ArtnetClient::Connect(){
    
    /*
    if(!Ethernet.connected()){
        Ethernet.begin(this->staticIP,INADDR_NONE,INADDR_NONE,this->staticSubnet);
        delay(100);
        Ethernet.begin(DHCP_TIMEOUT_WINDOW);
        delay(500);
        return true;
    } else if(!this->udpStarted){
        this->udpClient = NetworkUDP();
        this->udpClient.begin(Ethernet.localIP(),ARTNET_LISTENING_PORT);
        this->udpStarted = true;
        return true;
    }*/
    if(!ETH.connected()){
        ETH.begin(ETH_PHY_TYPE,ETH_ADDR,ETH_MDC_PIN,ETH_MDIO_PIN,ETH_POWER_PIN,ETH_CLK_MODE);
        ETH.macAddress(mac);
        delay(100);
        ETH.begin();  // Start Ethernet
        ETH.config(this->staticIP, IPAddress("0.0.0.0"), this->staticSubnet, IPAddress("0.0.0.0"));;
        delay(500);
    } else if(!udpStarted){
        this->udpClient.begin(ARTNET_LISTENING_PORT);
        this->udpStarted = true;
        return true;
    }
    return false;
}

void ArtnetClient::Receive(){
    int dataLength = this->udpClient.available();
    if(dataLength > 0){
        if(Serial){Serial.println("data received");}
    }
}

void ArtnetClient::DisConnect(){
    //this->udpClient.flush();
    //this->udpClient.stop();
    //Ethernet.end();
}
void ArtnetClient::PrintStatus(){

}