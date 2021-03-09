#ifndef WIFIZ_H
#define WIFIZ_H

#include <WiFi.h>

class Wifiz {
    const char * ssid;
    const char * password;
public:

    Wifiz(const char * ssid, const char * password) : ssid(ssid), password(password) {}

    IPAddress begin() {
        Serial.println("WiFi begin");
        WiFi.begin(ssid, password);
        while(WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }

        Serial.print("WiFi IP address: ");
        Serial.println(WiFi.localIP());

        return WiFi.localIP();
    }

};

#endif