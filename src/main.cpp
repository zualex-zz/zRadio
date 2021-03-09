/*
  Streaming Music from Bluetooth, Web radio
*/

#include <Arduino.h>
#include <driver/i2s.h>

// #define FLOAT_FFT
#ifdef FLOAT_FFT
#include "fftz.h"
#else
#include "fix-fftz.h"
#endif

#include "bluetoothz.h"
#include "wifiz.h"
#include "web-radioz.h"
// #include "web-socketz.h"
#include "web-serverz.h"

#define LED_STRIPE
#ifdef LED_STRIPE
#include "fastledz.h"
#endif

// #define LOCAL_INTERFACE
#ifdef LOCAL_INTERFACE
#include "oledz.h"
#include "encoderz.h"
#endif

static const char * ssid = "myWifiName";
static const char * password = "myWifiPassword";
static const char * zHome = "z-home";

int I2S_DOUT = 25;
int I2S_BCLK = 27;
int I2S_LRC = 26;

static const i2s_pin_config_t pins = {
  .bck_io_num = I2S_BCLK,
  .ws_io_num =  I2S_LRC,
  .data_out_num = I2S_DOUT
};

Bluetoothz bluetoothz;

#ifdef FLOAT_FFT
Fftz fftz;
#else
FixFftz fixFftz;
#endif
#ifdef LED_STRIPE
FastLedz fastLedz(13, 172, 10);
#endif

Wifiz wifiz(ssid, password);
WebRadioz webRadioz;
// WebSocketz webSocketz;
WebServerz webServerz;

#ifdef LOCAL_INTERFACE
Oledz oledz;
Encoderz* encoderz;
#endif

uint32_t i;
const uint8_t *data;
uint32_t len;
boolean dataChanged = false;

static int16_t sample_l_int = 0;
static int16_t sample_r_int = 0;

short mode = 0; // 0 webradio, 1 bluetooth

void az_stream(int16_t* m_outBuff, int16_t m_outBuffLen, uint8_t bitsPerSample, uint8_t channels) {

#ifdef FLOAT_FFT
  if (fftz.queueEmpty()) {
    fftz.minIn = 1024;
    fftz.maxIn = 0;

    for (i = 0; i < fftz.SAMPLES; i++) {
      sample_l_int  = m_outBuff[i*2];
      sample_r_int = m_outBuff[i*2 + 1];

      fftz.vReal[i] = (sample_l_int + sample_r_int) / 2.0f;
      fftz.vImag[i] = 0;

      if (fftz.vReal[i] > fftz.maxIn) {
          fftz.maxIn = fftz.vReal[i];
      }
      if (fftz.vReal[i] < fftz.minIn) {
          fftz.minIn = fftz.vReal[i];
      }
    }

    fftz.queueSend();
  // } else {
  //   Serial.println("queueFull");
  }
#else
  fixFftz.minIn = 1024;
  fixFftz.maxIn = 0;

  for (i = 0; i < fixFftz.SAMPLES; i++) {
    sample_l_int  = m_outBuff[i*2];
    sample_r_int = m_outBuff[i*2 + 1];

    fixFftz.vReal[i] = (sample_l_int + sample_r_int) / 2.0f;
    fixFftz.vImag[i] = 0;

    if (fixFftz.vReal[i] > fixFftz.maxIn) {
        fixFftz.maxIn = fixFftz.vReal[i];
    }
    if (fixFftz.vReal[i] < fixFftz.minIn) {
        fixFftz.minIn = fixFftz.vReal[i];
    }
  }

  fixFftz.executeFFT();

#endif
}


// Bluetooth stream with fft in another task
// void streamReader(const uint8_t *data, uint32_t len) {
//   // Serial.print("streamReader ");Serial.print(len);
//   if (fftz.queueEmpty()) {
//     fftz.minIn = 1024, fftz.maxIn = 0;
//     //log_e("Queue is empty, adding new item");
//     int byteOffset = 0;
//     for (i = 0; i < fftz.SAMPLES; i++) {
//       sample_l_int = (int16_t)(((*(data + byteOffset + 1) << 8) | *(data + byteOffset)));
//       sample_r_int = (int16_t)(((*(data + byteOffset + 3) << 8) | *(data + byteOffset +2)));
//       fftz.vReal[i] = (sample_l_int + sample_r_int) / 2.0f;;
//       fftz.vImag[i] = 0;
//       byteOffset = byteOffset + 4;

//       if (in > fftz.maxIn) {
//           fftz.maxIn = in;
//       }
//       if (in < fftz.minIn) {
//           fftz.minIn = in;
//       }
//     }
//      fftz.queueSend();
//   }
// }



void streamReader(const uint8_t *dataz, uint32_t lenz) {
  data = dataz;
  len = lenz;
  dataChanged = true;
}


// String json;

void showSignal(const uint8_t *data, uint32_t len) {

  // try {
#ifdef FLOAT_FFT
    fftz.minIn = 1024, fftz.maxIn = 0;
    int byteOffset = 0;
    for (i = 0; i < fftz.SAMPLES; i++) {
      sample_l_int = (int16_t)(((*(data + byteOffset + 1) << 8) | *(data + byteOffset)));
      sample_r_int = (int16_t)(((*(data + byteOffset + 3) << 8) | *(data + byteOffset +2)));
      fftz.vReal[i] = (sample_l_int + sample_r_int) / 2.0f;;
      fftz.vImag[i] = 0;
      byteOffset = byteOffset + 4;

      if (fftz.vReal[i] > fftz.maxIn) {
          fftz.maxIn = fftz.vReal[i];
      }
      if (fftz.vReal[i] < fftz.minIn) {
          fftz.minIn = fftz.vReal[i];
      }
    }

    fftz.executeFFT();
#else
    fixFftz.minIn = 1024, fixFftz.maxIn = 0;
    int byteOffset = 0;
    for (i = 0; i < fixFftz.SAMPLES; i++) {
      sample_l_int = (int16_t)(((*(data + byteOffset + 1) << 8) | *(data + byteOffset)));
      sample_r_int = (int16_t)(((*(data + byteOffset + 3) << 8) | *(data + byteOffset +2)));
      fixFftz.vReal[i] = (sample_l_int + sample_r_int) / 2.0f;;
      fixFftz.vImag[i] = 0;
      byteOffset = byteOffset + 4;

      if (fixFftz.vReal[i] > fixFftz.maxIn) {
          fixFftz.maxIn = fixFftz.vReal[i];
      }
      if (fixFftz.vReal[i] < fixFftz.minIn) {
          fixFftz.minIn = fixFftz.vReal[i];
      }
    }

  fixFftz.executeFFT();
#endif
      
    //   // json = "[";
    //   // for (int i = 0; i < 8; i++) {
    //   //   if (i > 0) {
    //   //     json +=", ";
    //   //   }
    //   //   json += "{\"bin\":";
    //   //   json += "\"" + labels[i] + "\"";a
    //   //   json += ", \"value\":";
    //   //   json += String(fftBands[i]);
    //   //   json += "}"; 
    //   // }
    //   // json += "]";

    // Serial.print("bands: ");
    // Serial.println(json);

    // webSocketz.broadcastTXT(json);

}


String bluetooth(String value) {
  // webRadioz.stop();
  // bluetoothz.begin(zHome, pins, streamReader);
  // Serial.print("Free heap: ");Serial.println(ESP.getFreeHeap());
  pref.putShort("mode", 1);
  ESP.restart();
  return "";
}

String webRadio(String value) {
  // bluetoothz.stop();
  // fftz.startTask();
  // webRadioz.begin(pins, NULL /* webSocketz.broadcastTXT */);
  // Serial.print("Free heap: ");Serial.println(ESP.getFreeHeap());
  pref.putShort("mode", 0);
  ESP.restart();
  return "";
}

String enableFft(String value) {
  Serial.print("enableFft: ");
  Serial.println(value);
  #ifdef FLOAT_FFT
  fftz.enabled = value == "true";
  #else
  fixFftz.enabled = value == "true";
  #endif
  return value;
}

#ifdef LED_STRIPE
String nextPalette(String value) {
  fastLedz.nextPalette();
  return "";
}

String prevPalette(String value) {
  fastLedz.prevPalette();
  return "";
}
#endif

int32_t maxAmplitude = 30000; //FIXME 
int32_t maxAmplitudeFft = 500000; //FIXME 
// int32_t noise = 2000; 

// char* topMenu = "";
// int32_t topMenuValue;

#ifdef LOCAL_INTERFACE
const uint8_t menuSize = 7;
std::array<const char*, menuSize> menuLabels = { "Station", "Volume", "Noise", "Amplitude", "Amplitude Fft", "Min", "Max" };
std::array<int32_t, menuSize> menuValues;
std::array<int32_t, menuSize> menuScale = { 1, 1, 50, 2500, 10000, 0, 0 };
uint8_t menuCurrent = 0;
boolean menuEdit = false;
#endif

void renderFft() {
  // char* top = "";
  // char* bottom = "";
  // if (webRadioz.isPlaying) {
  //   top = webRadioz.station;
  //   bottom = webRadioz.streamtitle;
  // } else {
  //   sprintf(top, "%d", fftz.minIn);
  //   sprintf(bottom, "%d", fftz.maxIn);
  // }
  #ifdef LOCAL_INTERFACE
  menuValues = { webRadioz.cur_station, webRadioz.cur_volume, fftz.noise, maxAmplitude, maxAmplitudeFft, fftz.minIn, fftz.maxIn };
  #endif

  #ifdef FLOAT_FFT
  if (fftz.enabled) {
    #ifdef LED_STRIPE
    fastLedz.drawFft(fftz.peak, fftz.PEAK_SAMPLES, &maxAmplitudeFft);
    #endif
    #ifdef LOCAL_INTERFACE
    oledz.drawFft(fftz.peak, fftz.PEAK_SAMPLES, webRadioz.station, webRadioz.streamtitle, menuLabels.at(menuCurrent), &menuValues.at(menuCurrent), &maxAmplitudeFft);
    #endif
  } else {
    #ifdef LED_STRIPE
    fastLedz.drawBands(fftz.vReal, fftz.SAMPLES);
    #endif
    #ifdef LOCAL_INTERFACE
    oledz.drawBands(fftz.vReal, fftz.SAMPLES, webRadioz.station, webRadioz.streamtitle, menuLabels.at(menuCurrent), &menuValues.at(menuCurrent), &maxAmplitude);
    #endif
  }
#else
if (fixFftz.enabled) {
    #ifdef LED_STRIPE
    // fastLedz.drawFlow(fixFftz.vReal, fixFftz.vImag);
    fastLedz.drawFlow(&fixFftz.band);
    #endif
  } else {
    #ifdef LED_STRIPE
    fastLedz.drawPeaks(fixFftz.vReal, fixFftz.maxIn, fixFftz.SAMPLES);
    #endif
  }
#endif
}

#ifdef LOCAL_INTERFACE
volatile void rotationHandlerz(boolean up/* byte encoderPos */) {
  if (!menuEdit) {
    if (up && menuCurrent+1 < menuSize) {
      menuCurrent++;
    } else if (!up && menuCurrent-1 >= 0){
      menuCurrent--;
    }
    // fftz.refreshMenu();
  } else {
    // if (menuLabels.at(menuCurrent) == "Station") {
    if (menuCurrent == 0) {
      // if (up) {
      //   webRadioz.next("");
      // } else {
      //   webRadioz.prev("");
      // }
    // } else if (menuLabels.at(menuCurrent) == "Volume") {
    } else if (menuCurrent == 1) {
      if (up) {
        webRadioz.volumeUp();
      } else {
        webRadioz.volumeDown();
      }
    // } else if (menuLabels.at(menuCurrent) == "Noise") {
    } else if (menuCurrent == 2) {
      if (up) {
        fftz.noise+= menuScale.at(menuCurrent);
      } else {
        fftz.noise-= menuScale.at(menuCurrent);
      }
    // } else if (menuLabels.at(menuCurrent) == "Amplitude") {
    } else if (menuCurrent == 3) {
      if (up) {
        maxAmplitude+= menuScale.at(menuCurrent);
      } else {
        maxAmplitude-= menuScale.at(menuCurrent);
      }
    // } else if (menuLabels.at(menuCurrent) == "Amplitude Fft") {
    } else if (menuCurrent == 4) {
      if (up) {
        maxAmplitudeFft+= menuScale.at(menuCurrent);
      } else {
        maxAmplitudeFft-= menuScale.at(menuCurrent);
      }
    }
  }
}

volatile void pressedHandlerz() {
  menuEdit = !menuEdit;
  // Serial.print("pressedHandlerz");
}
#endif



void setup() {
  Serial.begin(115200);
  
  Serial.print("Total heap: ");Serial.println(ESP.getHeapSize());
  Serial.print("Free heap: ");Serial.println(ESP.getFreeHeap());
  Serial.print("Total PSRAM: ");Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM: ");Serial.println(ESP.getFreePsram());
  #ifdef LED_STRIPE
  fastLedz.begin();
  #endif
  wifiz.begin();

  webServerz.begin();
  webServerz.on("/bluetooth", bluetooth);
  webServerz.on("/webRadio", webRadio);
  webServerz.on("/volume", webRadioz.setVolume);
  webServerz.on("/next", webRadioz.next);
  webServerz.on("/prev", webRadioz.prev);
  webServerz.on("/station", webRadioz.addStation);
  webServerz.on("/fft", enableFft);
  #ifdef LED_STRIPE
  webServerz.on("/nextPalette", nextPalette);
  webServerz.on("/prevPalette", prevPalette);
  #endif

  #ifdef FLOAT_FFT
  fftz.begin(renderFft);
  #else
  fixFftz.begin(renderFft);
  #endif

  #ifdef LOCAL_INTERFACE
  oledz.begin(); 
  encoderz->begin(rotationHandlerz, pressedHandlerz);
  #endif

  pref.begin(zHome, false);
  if (pref.getShort("mode", 1000) == 1000){ // if that: pref was never been initialized
    pref.putShort("mode", 0);
  }
  // else{ // get the stored values
  mode = pref.getShort("mode");

  if (mode == 0) {
    // fftz.startTask();
    webRadioz.begin(pins, NULL /* webSocketz.broadcastTXT */);
  } else if (mode == 1) {
    bluetoothz.begin(zHome, pins, streamReader);
    pref.putShort("mode", 0);
  }

}

byte oldEncPos = 0;

void loop() {
  webRadioz.loop();
  if (dataChanged == true) {
    dataChanged = false;
    showSignal(data, len);
  }
  // webSocketz.loop();
  webServerz.handleClient();
}
