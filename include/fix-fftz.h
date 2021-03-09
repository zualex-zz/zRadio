#ifndef FIXFFTZ_H
#define FIXFFTZ_H

// #include <arduinoFFT.h> 
#include <fix_fft.h>
#include "utilz.h"

class FixFftz {
    
    // static arduinoFFT* FFT;
    // static const double SAMPLING_FREQUENCY;
    // static QueueHandle_t queue;

    public:
    static const uint16_t SAMPLING_FREQUENCY = 44100;
    static const uint16_t SAMPLES = 512;
    static short vReal[SAMPLES];
    static short vImag[SAMPLES];  
    static short minIn;
    static short maxIn;
    // static int32_t noise;

    // static const uint8_t PEAK_SAMPLES = 8;
    // static int32_t peak[PEAK_SAMPLES];
    static Band band;
    static boolean enabled;

    static void (*renderCallback)();

    // static void createBands(int i, int dsize) {
    //     uint8_t band = 0;
    //     if (i <= 2) {
    //         band =  0; // 125Hz
    //     } else if (i <= 5) {
    //         band =   1; // 250Hz
    //     } else if (i <= 7)  {
    //         band =  2; // 500Hz
    //     } else if (i <= 15) {
    //         band =  3; // 1000Hz
    //     } else if (i <= 30) {
    //         band =  4; // 2000Hz
    //     } else if (i <= 53) {
    //         band =  5; // 4000Hz
    //     } else if (i <= 106) {
    //         band =  6;// 8000Hz
    //     } else {
    //         band = 7;
    //     }
    //     if (dsize > peak[band])
    //     {
    //         peak[band] = dsize;
    //     }
    // }

    static void executeFFT() {
        
        if (enabled) {

            // FFT->Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
            // FFT->Compute(FFT_FORWARD);
            // FFT->ComplexToMagnitude();

            // for (uint8_t band = 0; band < PEAK_SAMPLES; band++) {
            //     peak[band] = 0;
            // }

            // minIn = 1024, maxIn = 0;

            // for (int i = 2; i < (SAMPLES / 2); i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
            //     // if (vReal[i] > /* noise */2000) { // Add a crude noise filter, 10 x amplitude or more
            //         createBands(i, (int)vReal[i] /* / amplitude */);
            //     // }

            //     if (vReal[i] > maxIn) {
            //         maxIn = vReal[i];
            //     }
            //     if (vReal[i] < minIn) {
            //         minIn = vReal[i];
            //     }
            // }

            short m = 9;
            short inverse = 0;

            fix_fft(vReal, vImag, m, inverse);                            //perform the FFT on data

            const unsigned short stepFrequency = SAMPLING_FREQUENCY / SAMPLES;

            band.bass = 0;
            band.mid = 0;
            band.treble = 0;

            for (int i = 1; i < (SAMPLES / 2); i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
                short val =  std::abs(vImag[i]);
                val = map(val, 0, maxIn/2, 0, 255);
                if (i * stepFrequency < 500) { // bass
                    if (band.bass < val) {
                        band.bass = val;
                    }
                } else if (i * stepFrequency < 2000) { // mid
                    if (band.mid < val) {
                        band.mid = val;
                    }
                } else { // treble
                    if (band.treble < val) {
                        band.treble = val;
                    }
                }
            }
            
        }
        renderCallback();
    }

    // static void fftTask(void * parameter) {
    //     int item = 0;
    //     for(;;) {
    //         if (uxQueueMessagesWaiting(queue) > 0) {
    //                 executeFFT();
    //             xQueueReceive(queue, &item, 0);
    //         }
    //     }
    // }

    static void begin(void (*renderCallbackz)(/* int32_t*, uint8_t, int16_t*, int16_t* */) ) {

        // FFT = new arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY); /* Create FFT object */  

        renderCallback = renderCallbackz;
        enabled = true;
    }

    // static void startTask() {
    //     // The queue is used for communication between A2DP callback and the FFT processor
    //     queue = xQueueCreate( 1, sizeof( int ) );
    //     if(queue == NULL){
    //         Serial.println("Error creating the queue");
    //     }

    //     // This task will process the data acquired by the 
    //     // Bluetooth audio stream
    //     xTaskCreatePinnedToCore(
    //     fftTask,          // Function that should be called
    //     "FFT Renderer",     // Name of the task (for debugging)
    //     10000,              // Stack size (bytes)
    //     NULL,               // Parameter to pass
    //     1,                  // Task priority
    //     NULL,               // Task handle
    //     1                   // Core you want to run the task on (0 or 1)
    //     );

    // }

    // static boolean queueEmpty() {
    //     return uxQueueMessagesWaiting(queue) == 0;
    // }

    // static boolean queueSend() {
    //     int item = 0;
    //     return xQueueSend(queue, &item, portMAX_DELAY);
    // }

};

// arduinoFFT* Fftz::FFT;
const uint16_t FixFftz::SAMPLING_FREQUENCY;
// QueueHandle_t Fftz::queue;

const uint16_t FixFftz::SAMPLES;
short FixFftz::vReal[SAMPLES];
short FixFftz::vImag[SAMPLES]; 
short FixFftz::minIn;
short FixFftz::maxIn;
// int32_t Fftz::noise = 2000;

// const uint8_t Fftz::PEAK_SAMPLES;
// int32_t Fftz::peak[PEAK_SAMPLES];
Band FixFftz::band;
boolean FixFftz::enabled;

void (*FixFftz::renderCallback)();

#endif