#ifndef OLEDZ_H
#define OLEDZ_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "utilz.h"

class Oledz {
    uint8_t addr;
    uint8_t width;
    uint8_t height;
    Adafruit_SSD1306* display;
    static const int16_t oledBandsLen = 128; 
    double oledBands[oledBandsLen];
    char ylim = 32; // 60;
    // char x = 0;

public:

    const char* menu = "";

    Oledz(uint8_t addrz = 0x3C, uint8_t widthz = 128, uint8_t heightz = 64) {
        addr = addrz;
        width = widthz;
        height = heightz;
    }

    void begin() {
        Wire.begin(21, 22);
        display = new Adafruit_SSD1306(width, height, &Wire, -1);
        if(!display->begin(SSD1306_SWITCHCAPVCC, addr)) { // second on 0x3D
            Serial.println("Display allocation failed");
        }
        display->clearDisplay();
    }

    void showSplashScreen() {
        display->clearDisplay();
        display->setTextColor(SSD1306_WHITE);
        display->setCursor(0,0);
        display->print("z-home!");
        display->display();
    }

    void drawBands(double* bluetoothBuffer, uint16_t bluetoothBufferLen, char* top, char* bottom, const char* menu, int32_t* menuValue, const int32_t* bluetoothBufferAmplitude) {

        sampling(bluetoothBuffer, bluetoothBufferLen, oledBands, oledBandsLen);

        display->clearDisplay();

        if (bluetoothBufferAmplitude != 0) {
            for (uint8_t i = 0; i < oledBandsLen; i++) {
                display->drawLine(i, ylim, i, ylim - (oledBands[i] / *bluetoothBufferAmplitude * ylim), WHITE);
            };
        }

        // display.drawLine(0, 10, bands[0], 10, WHITE);
        // display.drawLine(0, 11, bands[0], 11, WHITE);

        display->setTextColor(SSD1306_WHITE);
        
        display->setCursor(0,0);
        display->print(menu);
        
        display->setCursor(0,8);
        display->print(*menuValue);

        display->setCursor(0,48);
        display->print(top);

        display->setCursor(0,56);
        display->print(bottom);

        display->display();
    }

    void drawFft(int32_t* bluetoothBuffer, uint16_t bluetoothBufferLen, char* top, char* bottom, const char* menu, int32_t* menuValue, const int32_t* bluetoothBufferAmplitude) {

        display->clearDisplay();

        // sampling(bluetoothBuffer, bluetoothBufferLen, oledBands, oledBandsLen);

        // if (bluetoothBufferAmplitude != 0) {
        //     for (uint8_t i = 0; i < oledBandsLen; i++) {
        //         display->drawLine(i + x, ylim, i + x, ylim - (oledBands[i] / *bluetoothBufferAmplitude * ylim), WHITE);
        //     };
        // }

        uint8_t barWidth = width / bluetoothBufferLen;

        if (bluetoothBufferAmplitude != 0) {
            for (uint8_t i = 0; i < bluetoothBufferLen; i++) {
                display->drawLine(i*barWidth, height, i*barWidth, height - (bluetoothBuffer[i] / *bluetoothBufferAmplitude * height), WHITE);
                // display->drawLine(i*barWidth, ylim, (i+1)*barWidth, ylim - (bluetoothBuffer[i] / *bluetoothBufferAmplitude * ylim), WHITE);
                // display->drawRect(i*barWidth, ylim - (bluetoothBuffer[i] / *bluetoothBufferAmplitude * ylim), barWidth, bluetoothBuffer[i] / *bluetoothBufferAmplitude * ylim, WHITE);
            };
        }

        // display.drawLine(0, 10, bands[0], 10, WHITE);
        // display.drawLine(0, 11, bands[0], 11, WHITE);

        display->setTextColor(SSD1306_WHITE);

        display->setCursor(0,0);
        display->print(menu);
        
        display->setCursor(0,8);
        display->print(*menuValue);

        display->setCursor(0,48);
        display->print(top);

        display->setCursor(0,56);
        display->print(bottom);
        
        display->display();
    }

};

#endif