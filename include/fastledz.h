#ifndef FASTLEDZ_H
#define FASTLEDZ_H

#include <FastLED.h>
#include "utilz.h"

#define LED_PIN     13 // 5
#define NUM_LEDS    172 // 50
#define BRIGHTNESS  10 // 25 // 64
#define LED_TYPE    WS2812B // WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int visualizationCounter = 0;
int32_t lastVisualizationUpdate = 0;


class FastLedz {
    uint8_t ledPin;
    uint16_t numLeds;
    uint8_t brightness;
    
    CRGBPalette16 palettes[10];
    int currentPalette = 0;
    TBlendType    currentBlending;

    static const int16_t bandsLen = NUM_LEDS; 
    double bands[bandsLen];

    uint32_t audioBuffer[NUM_LEDS];

    Band audioBufferz[NUM_LEDS];

    CRGB left_array[NUM_LEDS/2];
    CRGB right_array[NUM_LEDS/2];

public:

    FastLedz(uint8_t ledPinz, uint16_t numLedsz, uint8_t brightnessz ) {
        ledPin = ledPinz;
        numLeds = numLedsz;
        brightness = brightnessz;

        palettes[0] = equalizerPalette;
        palettes[1] = RainbowColors_p;
        palettes[2] = RainbowStripeColors_p;
        palettes[3] = HeatColors_p;
        palettes[4] = SetupPurpleAndGreenPalette();             
        // palettes[4] = SetupTotallyRandomPalette();              
        palettes[5] = SetupBlackAndWhiteStripedPalette();       
        palettes[6] = LavaColors_p;
        palettes[7] = CloudColors_p;
        palettes[8] = PartyColors_p;
        palettes[9] = myRedWhiteBluePalette_p;

        initAudioBuffer();
    }

    void begin() {
        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
        FastLED.setBrightness(  BRIGHTNESS );

        // limit my draw to 1,8A at 5v of power draw
        // FastLED.setMaxPowerInVoltsAndMilliamps(5, 1800); 
        FastLED.setMaxPowerInVoltsAndMilliamps(5, 800); 
    }

    void nextPalette() {
        currentPalette++;
    }

    void prevPalette() {
        currentPalette--;
    }


    // This function fills the palette with totally random colors.
    CRGBPalette16 SetupTotallyRandomPalette()
    {
        CRGBPalette16 palette;
        // Serial.println("SetupTotallyRandomPalette");
        for( int i = 0; i < 16; i++) {
            palette[i] = CHSV( random8(), 255, random8());
        }
        return palette;
    }

    // This function sets up a palette of black and white stripes,
    // using code.  Since the palette is effectively an array of
    // sixteen CRGB colors, the various fill_* functions can be used
    // to set them up.
    CRGBPalette16 SetupBlackAndWhiteStripedPalette()
    {
        CRGBPalette16 palette;
        // Serial.println("SetupBlackAndWhiteStripedPalette");
        // 'black out' all 16 palette entries...
        fill_solid( palette, 16, CRGB::Black);
        // and set every fourth one to white.
        palette[0] = CRGB::White;
        palette[4] = CRGB::White;
        palette[8] = CRGB::White;
        palette[12] = CRGB::White;

        return palette;
        
    }

    // This function sets up a palette of purple and green stripes.
    CRGBPalette16 SetupPurpleAndGreenPalette()
    {
        CRGBPalette16 palette;
        // Serial.println("SetupPurpleAndGreenPalette");
        CRGB purple = CHSV( HUE_PURPLE, 255, 255);
        CRGB green  = CHSV( HUE_GREEN, 255, 255);
        CRGB black  = CRGB::Black;
        
        palette = CRGBPalette16(
                                    green,  green,  black,  black,
                                    purple, purple, black,  black,
                                    green,  green,  black,  black,
                                    purple, purple, black,  black );
        return palette;
    }


    // This example shows how to set up a static color palette
    // which is stored in PROGMEM (flash), which is almost always more
    // plentiful than RAM.  A static PROGMEM palette like this
    // takes up 64 bytes of flash.
    const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
    {
        CRGB::Red,
        CRGB::Gray, // 'white' is too bright compared to red and blue
        CRGB::Blue,
        CRGB::Black,
        
        CRGB::Red,
        CRGB::Gray,
        CRGB::Blue,
        CRGB::Black,
        
        CRGB::Red,
        CRGB::Red,
        CRGB::Gray,
        CRGB::Gray,
        CRGB::Blue,
        CRGB::Blue,
        CRGB::Black,
        CRGB::Black
    };

    // see https://coolors.co/474647-2d7dd2-97cc04-eeb902-f18b02-f45d01
    const TProgmemPalette16 equalizerPalette PROGMEM =
    {
        0x474647,
        0x3A628D,
        0x2D7DD2,
        0x48919F,
        0x62A56B,
        0x7DB938,
        0x97CC04,
        0xADC804,
        0xC3C303,
        0xD9BE03,
        0xEEB902,
        0xEFAE02,
        0xF0A202,
        0xF18B02,
        0xF37402,
        0xF45D01
        
    };


    // Additional notes on FastLED compact palettes:
    //
    // Normally, in computer graphics, the palette (or "color lookup table")
    // has 256 entries, each containing a specific 24-bit RGB color.  You can then
    // index into the color palette using a simple 8-bit (one byte) value.
    // A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
    // is quite possibly "too many" bytes.
    //
    // FastLED does offer traditional 256-element palettes, for setups that
    // can afford the 768-byte cost in RAM.
    //
    // However, FastLED also offers a compact alternative.  FastLED offers
    // palettes that store 16 distinct entries, but can be accessed AS IF
    // they actually have 256 entries; this is accomplished by interpolating
    // between the 16 explicit entries to create fifteen intermediate palette
    // entries between each pair.
    //
    // So for example, if you set the first two explicit entries of a compact 
    // palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
    // the first sixteen entries from the virtual palette (of 256), you'd get
    // Green, followed by a smooth gradient from green-to-blue, and then Blue.


    // There are several different palettes of colors demonstrated here.
    //
    // FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
    // OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
    //
    // Additionally, you can manually define your own color palettes, or you can write
    // code that creates color palettes on the fly.  All are shown here.

    void ChangePalettePeriodically()
    {
        uint8_t secondHand = (millis() / 1000) % 60;
        static uint8_t lastSecond = 99;
        
        if( lastSecond != secondHand) {
            lastSecond = secondHand;
            if( secondHand ==  0)  { currentPalette = 0;                        currentBlending = LINEARBLEND; }
            if( secondHand == 10)  { currentPalette = 1;                        currentBlending = NOBLEND;  }
            if( secondHand == 15)  { currentPalette = 1;                        currentBlending = LINEARBLEND; }
            if( secondHand == 20)  { currentPalette = 2;                        currentBlending = LINEARBLEND; }
            if( secondHand == 25)  { currentPalette = 3;                        currentBlending = LINEARBLEND; }
            if( secondHand == 30)  { currentPalette = 4;                        currentBlending = NOBLEND; }
            if( secondHand == 35)  { currentPalette = 5;                        currentBlending = LINEARBLEND; }
            if( secondHand == 40)  { currentPalette = 6;                        currentBlending = LINEARBLEND; }
            if( secondHand == 45)  { currentPalette = 7;                        currentBlending = LINEARBLEND; }
            if( secondHand == 50)  { currentPalette = 8;                        currentBlending = NOBLEND;  }
            if( secondHand == 55)  { currentPalette = 9;                        currentBlending = LINEARBLEND; }
        }
    }

    static uint8_t getLedIndex(uint8_t x, uint8_t y) {
        //x = 7 - x;
        // if (y % 2 == 0) {
            return y * 8 + x;
        // } else {
        //     return y*8 + (7 - x);
        // }
    }

    static uint8_t getLedIndex16(uint8_t x, uint8_t y) {
        //x = 7 - x;
        // if (y % 2 == 0) {
            return y * 16 + x;
        // } else {
        //     return y*16 + (15 - x);
        // }
    }


    static void drawFft(int32_t* peak, uint8_t numBands, const int32_t* amplitude) {
        uint8_t intensity;

        FastLED.clear();
        FastLED.setBrightness(BRIGHTNESS);
        for (byte band = 0; band < numBands; band++) {
            // intensity = map(peak[band], 1, *amplitude, 0, 8);
            // for (int i = 0; i < 8; i++) {
            //     leds[getLedIndex(7 - i, 7 - band)] = (i >= intensity) ? CHSV(0, 0, 0) : CHSV(i * 16, 255, 255);
            // }
            intensity = map(peak[band], 1, *amplitude, 0, 16);
            for (int i = 0; i < 16; i++) {
                leds[getLedIndex16(15 - i, 7 - band)] = (i >= intensity) ? CHSV(0, 0, 0) : CHSV(i * 8, 255, 255);
            }
        }

        FastLED.show();

        // if ((millis() - lastVisualizationUpdate) > 2000) {
        //     // log_e("Fps: %f", visualizationCounter / ((millis() - lastVisualizationUpdate) / 1000.0));
        //     Serial.print("Fps ");Serial.print(visualizationCounter / ((millis() - lastVisualizationUpdate) / 1000.0));
        //     visualizationCounter = 0;
        //     lastVisualizationUpdate = millis();
        //     // hueOffset += 5;
        //     Serial.print(peak[0]);Serial.print(peak[1]);Serial.print(peak[2]);Serial.println(peak[3]);
        // }
        // visualizationCounter++;
    }
    

   
    void drawBands(double * bluetoothBuffer, uint16_t bluetoothBufferLen) {

        ChangePalettePeriodically();

        sampling(bluetoothBuffer, bluetoothBufferLen, bands, bandsLen);

        uint8_t brightness = 255;
        for (int16_t i = 0; i < bandsLen; i++) { // In the current design, 60Hz and noise
            leds[i] = ColorFromPalette( palettes[currentPalette], bands[i], brightness, currentBlending);
        };

        FastLED.show();

        // if ((millis() - lastVisualizationUpdate) > 2000) {
        //     Serial.println("Fps ");Serial.print(visualizationCounter / ((millis() - lastVisualizationUpdate) / 1000.0));
        //     visualizationCounter = 0;
        //     lastVisualizationUpdate = millis();
        // }
        // visualizationCounter++;
    }
       
    // void drawEq(double * fft, uint16_t fftLen, int16_t* maxIn) {

    //     currentPalette = 0;

    //     uint8_t pixelPerBand = 172 / fftLen;
    //     Serial.print("pixelPerBand: ");
    //     Serial.println(pixelPerBand);

    //     // uint8_t brightness = 255;
    //     for (uint8_t i = 0; i < fftLen; i++) {
    //         double val = fft[i];
    //         for (uint8_t j = 0; j < pixelPerBand; j++) {
    //             if (val > (*maxIn / pixelPerBand) * j) {
    //                 leds[i * pixelPerBand + j] = CRGB::Blue;
    //             } else {
    //                 leds[i * pixelPerBand + j] = CRGB::Black;
    //             }
    //         }
    //     };

    //     FastLED.show();
    // }





    void initAudioBuffer() {
        for (int i = 0; i < NUM_LEDS; i++) {
            audioBuffer[i] = 0;
            audioBufferz[i] = {0, 0, 0};
        }
    }


    void drawFlow(short* data, short* im) {

        //  Buffer arrays for Audio Analyzer Mode
        int spectrumValue[8];
        uint8_t mapValue[8];

        for (int i = 0; i < 8; i++) {
            spectrumValue[i] = 0;
            for (int j = 0; j < 8; j++) {
                int dat = sqrt(data[i * 8 + j] * data[i * 8 + j] + im[i * 8 + j] * im[i * 16 + j]); //filter out noise and hum;
                if (dat > spectrumValue[i])
                    spectrumValue[i] = dat;
            }
            mapValue[i] = map(spectrumValue[i], 0, 30, 0, 255);
            // #if DEBUG
            // Serial.print(mapValue[i]);
            // Serial.print(" ");
            // #endif

            // Shift LED values forward
            for (volatile int i = NUM_LEDS - 1; i > 0; i--) {
                audioBuffer[i] = audioBuffer[i - 1];
            }

            audioBuffer[0] = mapValue[5]; //RED
            audioBuffer[0] = audioBuffer[0] << 16;
            audioBuffer[0] |= ((mapValue[2] / 2) << 8); //GREEN
            audioBuffer[0] |= (mapValue[4] / 4);       //BLUE
            //Send new LED values to WS2812


            for ( int i = 0; i < NUM_LEDS; i++) {
                // strip.setPixelColor(i, strip.Color(audioBuffer[i] >> 16, (audioBuffer[i] >> 8)&B11111111, audioBuffer[i]&B11111111));
                leds[i] = CRGB(audioBuffer[i] >> 16, (audioBuffer[i] >> 8)&B11111111, audioBuffer[i]&B11111111);
            }
            FastLED.show();
        }
    }

    void drawFlow(Band* band) {

        // Shift LED values forward
        for (volatile int i = NUM_LEDS - 1; i > 0; i--) {
            audioBufferz[i] = audioBufferz[i - 1];
        }

        audioBufferz[0] = *band;

        for ( int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(audioBufferz[i].bass, audioBufferz[i].mid, audioBufferz[i].treble);
        }
        FastLED.show();

    }

    void drawPeaks(short* vReal, const short maxIn, const uint16_t samples) {
        
        // Some example procedures showing how to display to the pixels:
        //Set the hue (0-255) and 24-bit color depending on left channel value
        // byte hue_left = constrain(map(analogRead(left_channel), 0, 400, 0, 255), 0, 255);
        byte hue_left = map(vReal[0], 0, maxIn, 0, 255);
        // uint32_t color_left = Wheel(hue_left);
        CRGB color_left = ColorFromPalette( palettes[currentPalette], hue_left, brightness, currentBlending);
        
        Serial.print("vReal[0]: ");Serial.println(vReal[0]);
        Serial.print("hue_left: ");Serial.println(hue_left);
        // Serial.print("color_left: ");Serial.println(color_left.);

        //Set the hue (0-255) and 24-bit color depending on right channel value
        // byte hue_right = constrain(map(analogRead(right_channel), 0, 400, 0, 255), 0, 255);
        // uint32_t color_right = Wheel(hue_right);
        CRGB color_right = color_left;
        
        //Shift the current values.
        for (int i = 0; i < NUM_LEDS/2-1; i++)
        {
            left_array[i] = left_array[i+1];
            right_array[i] = right_array[i+1];
        }
        
        //Fill in the new value at the end of each array
        left_array[NUM_LEDS/2-1] = color_left;
        right_array[NUM_LEDS/2-1] = color_right;
        
        //Go through each Pixel on the strip and set its color
        for (int i=0; i<NUM_LEDS/2; i++)
        {
            //set pixel color
            // strip.setPixelColor(i, left_array[i]);
            // strip.setPixelColor(length-i-1, right_array[i]);
            leds[i] = left_array[i];
            leds[NUM_LEDS-i-1] = right_array[i];
        }


        //Display the new values
        // strip.show();
        FastLED.show();
        
        //sample delay
        // delay(40);

    }

};

#endif