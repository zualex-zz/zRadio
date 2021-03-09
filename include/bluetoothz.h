#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <BluetoothA2DPSink.h>
static BluetoothA2DPSink* a2dp_sink = NULL;  
class Bluetoothz {
    
    // char* bluetoothName = "z-home";
    // i2s_pin_config_t pins;
public:

    // Bluetoothz(char* bluetoothNamez, i2s_pin_config_t pinz) : bluetoothName(bluetoothNamez), pins(pinz) {}

    static void begin(const char* bluetoothName, i2s_pin_config_t pins, void (*streamReader)(const uint8_t*, uint32_t)) {
        Serial.println("Bluetoothz begin");

        a2dp_sink = new BluetoothA2DPSink();

        a2dp_sink->set_pin_config(pins);
        a2dp_sink->set_stream_reader(streamReader);
        a2dp_sink->start((char*)bluetoothName);
    }

    static void stop() {
        if (a2dp_sink) {
            a2dp_sink->~BluetoothA2DPSink();
            a2dp_sink = NULL;
        }
    }

};

#endif