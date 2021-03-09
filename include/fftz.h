#ifndef FFTZ_H
#define FFTZ_H

#include <arduinoFFT.h> 

class Fftz {
    
    static arduinoFFT* FFT;
    static const double SAMPLING_FREQUENCY;
    static QueueHandle_t queue;

    public:
    
    static const uint16_t SAMPLES = 512;
    static double vReal[SAMPLES];
    static double vImag[SAMPLES];  
    static int32_t minIn;
    static int32_t maxIn;
    static int32_t noise;

    static const uint8_t PEAK_SAMPLES = 8;
    static int32_t peak[PEAK_SAMPLES];
    static boolean enabled;

    static void (*renderCallback)(/* int32_t*, uint8_t, int16_t*, int16_t* */);

    static void createBands(int i, int dsize) {
        uint8_t band = 0;
        if (i <= 2) {
            band =  0; // 125Hz
        } else if (i <= 5) {
            band =   1; // 250Hz
        } else if (i <= 7)  {
            band =  2; // 500Hz
        } else if (i <= 15) {
            band =  3; // 1000Hz
        } else if (i <= 30) {
            band =  4; // 2000Hz
        } else if (i <= 53) {
            band =  5; // 4000Hz
        } else if (i <= 106) {
            band =  6;// 8000Hz
        } else {
            band = 7;
        }
        // int dmax = amplitude;
        // if (dsize > dmax)
        //     dsize = dmax;
        if (dsize > peak[band])
        {
            peak[band] = dsize;
        }
    }

    static void executeFFT() {
        
        if (enabled) {

            // Serial.print("FFT ");
            // Serial.print(vReal[0]);Serial.print(" ");
            // Serial.print(vReal[1]);Serial.print(" ");
            // Serial.print(vReal[2]);Serial.print(" ");
            // Serial.print(vReal[3]);Serial.print(" ");
            // Serial.print(vReal[4]);Serial.print(" ");
            // Serial.print(vReal[5]);Serial.print(" ");
            // Serial.print(vReal[6]);Serial.print(" ");
            // Serial.println(vReal[7]);

            FFT->Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
            FFT->Compute(FFT_FORWARD);
            FFT->ComplexToMagnitude();

            for (uint8_t band = 0; band < PEAK_SAMPLES; band++) {
                peak[band] = 0;
            }

            minIn = 1024, maxIn = 0;

            for (int i = 2; i < (SAMPLES / 2); i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
                // if (vReal[i] > /* noise */2000) { // Add a crude noise filter, 10 x amplitude or more
                    createBands(i, (int)vReal[i] /* / amplitude */);
                // }

                if (vReal[i] > maxIn) {
                    maxIn = vReal[i];
                }
                if (vReal[i] < minIn) {
                    minIn = vReal[i];
                }
            }

            // Release handle
            // xQueueReceive(queue, &item, 0);

            // Serial.print("peak ");
            // Serial.print(peak[0]);Serial.print(" ");
            // Serial.print(peak[1]);Serial.print(" ");
            // Serial.print(peak[2]);Serial.print(" ");
            // Serial.print(peak[3]);Serial.print(" ");
            // Serial.print(peak[4]);Serial.print(" ");
            // Serial.print(peak[5]);Serial.print(" ");
            // Serial.print(peak[6]);Serial.print(" ");
            // Serial.println(peak[7]);

        // } else {
            
        }
        renderCallback();
    }

    static void fftTask(void * parameter) {
        int item = 0;
        for(;;) {
            if (uxQueueMessagesWaiting(queue) > 0) {
                // xQueuePeek(queue, &item, portMAX_DELAY);
                // Serial.print("qi "); Serial.println(item);
                // if (item == 0) {
                    executeFFT();
                // } else {
                //     renderCallback();
                // }
                // Release handle
                xQueueReceive(queue, &item, 0);
            }
        }
    }

    static void begin(void (*renderCallbackz)(/* int32_t*, uint8_t, int16_t*, int16_t* */) ) {

        FFT = new arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY); /* Create FFT object */  

        renderCallback = renderCallbackz;
        enabled = true;
    }

    static void startTask() {
        // The queue is used for communication between A2DP callback and the FFT processor
        queue = xQueueCreate( 1, sizeof( int ) );
        if(queue == NULL){
            Serial.println("Error creating the queue");
        }

        // This task will process the data acquired by the 
        // Bluetooth audio stream
        xTaskCreatePinnedToCore(
        fftTask,          // Function that should be called
        "FFT Renderer",     // Name of the task (for debugging)
        10000,              // Stack size (bytes)
        NULL,               // Parameter to pass
        1,                  // Task priority
        NULL,               // Task handle
        1                   // Core you want to run the task on (0 or 1)
        );

    }

    // static void stop() {

    // }

    static boolean queueEmpty() {
        return uxQueueMessagesWaiting(queue) == 0;
    }

    static boolean queueSend() {
        int item = 0;
        return xQueueSend(queue, &item, portMAX_DELAY);
    }

    // static boolean refreshMenu() {
    //     int item = 1;
    //     return xQueueSend(queue, &item, portMAX_DELAY);
    // }

};

arduinoFFT* Fftz::FFT;
const double Fftz::SAMPLING_FREQUENCY = 44100;
QueueHandle_t Fftz::queue;

const uint16_t Fftz::SAMPLES;
double Fftz::vReal[SAMPLES];
double Fftz::vImag[SAMPLES]; 
int32_t Fftz::minIn;
int32_t Fftz::maxIn;
int32_t Fftz::noise = 2000;

const uint8_t Fftz::PEAK_SAMPLES;
int32_t Fftz::peak[PEAK_SAMPLES];
boolean Fftz::enabled;

void (*Fftz::renderCallback)();

#endif