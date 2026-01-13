/*
 * HttpServer.h
 *
 *  Created on: December 27, 2023
 *      Author: wvanvliet
 */
#include "Arduino.h"
#include "globals.h"
#include <map>
#include <vector>
#include <EthernetESP32.h>
//#include <WebServer_WT32_ETH01.h>
#include <PerformanceData.h>

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#define HTTP_PORT 80

#define MIME_HTML  "text/html"
#define MIME_CSS   "text/css"
#define MIME_JS    "text/javascript"
#define MIME_GIF   "image/gif"
#define MIME_ICO   "image/vnd.microsoft.icon"
#define MIME_JPEG  "image/jpeg"
#define MIME_PNG   "image/png"
#define MIME_SVG   "image/svg+xml"
#define MIME_WEBP  "image/webp"
#define MIME_JSON  "application/json"
#define MIME_MIDI  "audio/midi"
#define MIME_MP3   "audio/mpeg"
#define MIME_TTF   "font/ttf"
#define MIME_WOFF  "font/woff"
#define MIME_WOFF2 "font/woff2"
#define MIME_TEXT  "text/plain"

#define RESPONSE_OK           "200 OK"
#define RESPONSE_NO_CONTENT   "204 NO CONTENT"
#define RESPONSE_BAD_REQUEST  "400 BAD REQUEST"
#define RESPONSE_NOT_FOUND    "404 NOT FOUND"
#define RESPONSE_UNSOPPORTED  "415 UNSUPPORTED MEDIA TYPE"
#define RESPONSE_TEAPOT       "418 IM A TEAPOT"
#define RESPONSE_TOO_MANY     "429 TOO MANY REQUESTS"

#define REQUEST_GET  "GET"
#define REQUEST_POST "POST" 

class HttpServer{
    public:
        HttpServer();
        void Start();
        void Listen();
    private:
        void sendPerformanceData(NetworkClient client);
        void handleGETHTTPRequest(String url, NetworkClient client);
        void sendHTMLHeader(String responseCode,String contentType, NetworkClient client);
        void sendCorrectHeader(String fileType, NetworkClient client);
        String extractGETUrlFromHeader(String header);
        String extractPOSTValuesFromHeader(String header);
        String extractRequestTypeFromHeader(String header);
        String extractContentTypeFromHeader(String header);
        std::map<String,String> postDataToMap(String data);
        NetworkServer server;
        PerformanceData *performance;
};
#endif