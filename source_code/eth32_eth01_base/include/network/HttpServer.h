/*
 * HttpServer.h
 *
 *  Created on: December 27, 2023
 *      Author: wvanvliet
 */
#include "Arduino.h"
#include "globals.h"
#include <EthernetESP32.h>

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#define HTTP_PORT 80

class HttpServer{
    public:
        HttpServer();
        void Start();
        void Listen();
    private:
        NetworkServer server;
};
#endif