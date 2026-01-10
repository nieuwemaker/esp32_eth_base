#include <network/HttpServer.h>

HttpServer::HttpServer(){

}

void HttpServer::Start(){
    this->server.begin(HTTP_PORT);
}

void HttpServer::Listen(){
    NetworkClient client = this->server.accept();
    if(client){
        String clientData = "";
        while (client.available()) { 
            clientData += client.readString();
        }
        clientData.replace("\n","<br>");
        client.write("<html><body><p>");
        client.write(clientData.c_str());
        client.write("</p></body></html>\n");
        client.stop();
    }
}
