#include <network/HttpServer.h>
#include "network/http/eth_config.h" // config.html
#include "network/http/home.h" // home.html ROOT
#include "network/http/performance.h" // home.html ROOT

HttpServer::HttpServer(){
    this->performance = PerformanceData::GetInstance();
}

void HttpServer::Start(){
    this->server.begin(HTTP_PORT);
}

void HttpServer::Listen(){
    NetworkClient client = this->server.accept();
    if(client){
        // Step 1: Get the request and decipher what it wants
        String request = "";
        while (client.available()) { 
            request += client.readString();
        }
        String requestType = this->extractRequestTypeFromHeader(request);
        String getUrl      = this->extractGETUrlFromHeader(request);
        // step 2: Handle correctly as possible
        if(requestType == REQUEST_GET){
            this->handleGETHTTPRequest(getUrl,client);
        } else if(requestType == REQUEST_POST) {
            this->sendHTMLHeader(RESPONSE_OK,MIME_HTML, client);
            //std::map<String,String> postData = this->postDataToMap(request);
            client.write("<h1>POST DATA</h1>");
        }
        client.stop();
    }
}

void HttpServer::handleGETHTTPRequest(String url, NetworkClient client){
    if(url.equals("/config") || url.equals("/config.html")){
        this->sendHTMLHeader(RESPONSE_OK,MIME_HTML, client);
        client.write(HTML_CONTENT_CONFIG); // Server config page
    } else if(url.equals("/performance") || url.equals("/performance.html")){
        this->sendHTMLHeader(RESPONSE_OK,MIME_HTML, client);
        client.write(HTML_CONTENT_PERFORMANCE); // Server config page
    } else if(url.equals("/rqstperform.js")){
        //this->sendHTMLHeader(RESPONSE_OK,MIME_HTML, client);
        //client.write("<h1>RQSTPERFORM</h1>");
        this->sendPerformanceData(client);
    } else {
        this->sendHTMLHeader(RESPONSE_OK,MIME_HTML, client);
        client.write(HTML_CONTENT_HOME);
    }
}

void HttpServer::sendPerformanceData(NetworkClient client){
    this->sendHTMLHeader(RESPONSE_OK,MIME_JS, client);
    String html = "let data = { FPS: ";
    html = String(html + String(this->performance->LastFps));
    html = String(html + ", Cycles: ");
    html = String(html + String(this->performance->LastLoop));
    html = String(html + "};");
    client.write(html.c_str());
    //client.write("let data = {Cycles:50};");
}

void HttpServer::sendCorrectHeader(String fileType, NetworkClient client){
    if(fileType == "html" || fileType == "htm"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_HTML, client);
    } else if(fileType == "css"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_CSS, client);
    } else if(fileType == "js"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_JS, client);
    } else if(fileType == "ico"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_ICO, client);
    } else if(fileType == "gif"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_GIF, client);
    } else if(fileType == "jpg"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_JPEG, client);
    } else if(fileType == "png"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_PNG, client);
    } else if(fileType == "svg"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_SVG, client);
    } else if(fileType == "webp"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_WEBP, client);
    } else if(fileType == "ttf"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_TTF, client);
    } else if(fileType == "woff"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_WOFF, client);
    } else if(fileType == "woff2"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_WOFF2, client);
    } else if(fileType == "txt"){
        this->sendHTMLHeader(RESPONSE_OK,MIME_TEXT, client);
    } else {
        this->sendHTMLHeader(RESPONSE_UNSOPPORTED,MIME_HTML, client);
        client.printf("<html><body>requested file type %s is not supported (yet)</body></html>",fileType.c_str());
    }
}

String HttpServer::extractPOSTValuesFromHeader(String header){
    header = header.substring(header.indexOf("\n\r",0)+1);
    header.trim();
    return header;
}

String HttpServer::extractGETUrlFromHeader(String header){
    header = header.substring(4,header.indexOf("HTTP",0)-1);
    header.trim();
    return header;
}

String HttpServer::extractContentTypeFromHeader(String header){
    String strip = header.substring(header.indexOf("Content-Type:")+14);
    strip = strip.substring(0, strip.indexOf("\n"));
    strip.trim();
    return strip;
}

String HttpServer::extractRequestTypeFromHeader(String header){
    return header.substring(0,header.indexOf(" ",0));
}

void HttpServer::sendHTMLHeader(String responseCode, String contentType, NetworkClient client){
    client.printf("HTTP/1.1 %s\n",responseCode.c_str());
    client.println("Access-Control-Allow-Origin: *");
    client.printf("Content-Type: %s\n\n",contentType.c_str());
}

std::map<String,String> HttpServer::postDataToMap(String data){
    std::map<String,String> mapData;
        
    while(data.indexOf('&')> -1){
        String kvPair = data.substring(0,data.indexOf('&'));
        mapData.insert({kvPair.substring(kvPair.indexOf('=')), kvPair.substring(kvPair.indexOf('=')+1)});
        data = data.substring(0,data.indexOf('&')+1);
    }
    // last kv pair needs to be added here
    String kvPair = data.substring(0,data.indexOf('&'));
    mapData.insert({kvPair.substring(kvPair.indexOf('=')), kvPair.substring(kvPair.indexOf('=')+1)});
    return mapData;
}
