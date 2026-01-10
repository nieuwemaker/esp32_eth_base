#include <network/WifiNetwork.h>

WifiNetwork::WifiNetwork() : AbstractNetworkInterface(){

}

bool WifiNetwork::IsConnected(){
    return this->connStatus != CONNECT_STATUS_NONE;
}

IPAddress WifiNetwork::GetIP(){
    return WiFi.softAPIP();
}

bool WifiNetwork::Connect(){
    if(this->ssid != NULL){
       return this->connectWithWifi();
    } else if(!this->IsConnected()){
        return this->connectWithAP();
    }
    this->connStatus = CONNECT_STATUS_NONE;
    return false;
}

bool WifiNetwork::connectWithAP(){
    if(!WiFi.softAP(this->apSsid)){
        while (1);
    } else {
        this->MyIP       = WiFi.softAPIP();
        this->connStatus = CONNECT_STATUS_AP;
        return true;
    }
    return false;
}

bool WifiNetwork::connectWithWifi(){
    return false;
}

void WifiNetwork::DisConnect(){

}

void WifiNetwork::PrintStatus(){

}