#ifndef ENCORERZ_H
#define ENCORERZ_H

/**
 * Read rotatory encoder from clk pin GPIO34 and dt pin GPIO35
 */
class Encoderz {
    // uint8_t pinClk;
    // uint8_t pinDt;
    static uint8_t pinSw;

    static volatile byte reading;
    static volatile byte aFlag;
    static volatile byte bFlag;

    static volatile void (*rotationHandler)(boolean up/* byte encoderPos */);
    static volatile void (*pressedHandler)();

    public:

    static volatile byte encoderPos;

    // see: https://stackoverflow.com/questions/53802457/adapting-this-arduino-instructable-to-esp32
    
    // Encoder clk pin GPIO34 and dt pin GPIO35:
    // GPIO34 is binary 100
    // GPIO35 is binary 1000
    // both 1100 or 0xC

    static void /* IRAM_ATTR */ PinA()
    {
        cli();
        reading = GPIO_REG_READ(GPIO_IN1_REG) & 0xC;
        if (reading == B1100 && aFlag)
        {
            encoderPos--;
            bFlag = 0;
            aFlag = 0;
            if (rotationHandler) {
                rotationHandler(false/* encoderPos */);
            }
        }
        else if (reading == B1000)
            bFlag = 1;
        sei();
    }

    static void /* IRAM_ATTR */ PinB()
    {
        cli();
        reading = GPIO_REG_READ(GPIO_IN1_REG) & 0xC;
        if (reading == B1100 && bFlag)
        {
            encoderPos++;
            bFlag = 0;
            aFlag = 0;
            if (rotationHandler) {
                rotationHandler(true/* encoderPos */);
            }
        }
        else if (reading == B100)
            aFlag = 1;
        sei();
    }

    static void /* IRAM_ATTR */ swRising() {
        cli();
        if (pressedHandler) {
            pressedHandler();
        }
        sei();
    }


    // Encoderz(uint8_t pinClk = 34, uint8_t pinDt = 35, uint8_t pinSw = 39) : pinClk(pinClk), pinDt(pinDt), pinSw(pinSw) {
    static void begin(volatile void (*rotationHandlerz)(boolean up/* byte encoderPos */), volatile void (*pressedHandlerz)(), uint8_t pinClk = 34, uint8_t pinDt = 35, uint8_t pinSwz = 39) {
        rotationHandler = rotationHandlerz;
        pressedHandler = pressedHandlerz;
        pinSw = pinSwz;

        pinMode(pinClk, INPUT_PULLUP);
        pinMode(pinDt, INPUT_PULLUP);
        pinMode(pinSw, INPUT_PULLUP);
        
        attachInterrupt(digitalPinToInterrupt(pinClk), PinA, RISING);
        attachInterrupt(digitalPinToInterrupt(pinDt), PinB, RISING);
        attachInterrupt(digitalPinToInterrupt(pinSw), swRising, RISING);
    }

    // static int readSw() {
    //     return digitalRead(pinSw);
    // }


};

uint8_t Encoderz::pinSw;
volatile byte Encoderz::reading = 0;
volatile byte Encoderz::aFlag = 0;
volatile byte Encoderz::bFlag = 0;
volatile void (*Encoderz::rotationHandler)(boolean up/* byte encoderPos */);
volatile void (*Encoderz::pressedHandler)();

volatile byte Encoderz::encoderPos = 0;
// void IRAM_ATTR Encoderz::PinA();
// void IRAM_ATTR Encoderz::PinB();

#endif