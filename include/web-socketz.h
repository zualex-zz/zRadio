#ifndef WEBSOCKETZ_H
#define WEBSOCKETZ_H

#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

class WebSocketz {
    
    // WebSocketServerEvent webSocketServerEvent;
public:

    WebSocketz() {
        Serial.print("WebSocketz constructor");
    }

    void begin() {
        webSocket.begin();
        // webSocket.onEvent(webSocketEvent);   
    }

    // void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
    //   // Do something with the data from the client
    //   if(type == WStype_TEXT){
    //     Serial.print("webSocketEvent: ");
    //     Serial.println(payload); 
    //   }
    // }

    void static broadcastTXT(String json) {
        webSocket.broadcastTXT(json.c_str(), json.length());
    }

    void loop() {
        webSocket.loop();
    }

};

#endif