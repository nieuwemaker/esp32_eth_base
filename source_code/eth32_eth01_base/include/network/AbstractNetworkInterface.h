/*
 * NetworkController.h
 *
 *  Created on: December 27, 2023
 *      Author: wvanvliet
 */
#include "Arduino.h"
#include "globals.h"

#ifndef ABSTRACTNETWORKINTERFACE_H_
#define ABSTRACTNETWORKINTERFACE_H_

#define HOST_NAME "PP_"

#define DHCP_TIMEOUT_WINDOW 10'000

class AbstractNetworkInterface{
    public:
        AbstractNetworkInterface(){}
        bool 
            IsConnected(),
            Connect();
        void DisConnect(),
             PrintStatus();
        String GetLocalName(){ return HOST_NAME + MyIP; }
        IPAddress GetIP() {return this->MyIP; }

        IPAddress     MyIP;  
};
#endif