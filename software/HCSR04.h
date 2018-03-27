
#ifndef HCSR04_h
#define HCSR04_h

#include "FourSonicSensors.h"

class HCSR04 : public UltraSonicSensor {
    PinName trigPin = NC;
    PinName echoPin = NC;
    
    mbed::DigitalOut *trig  = nullptr;
    mbed::InterruptIn *echo = nullptr;
    
    mbed::Timer timer;
    mbed::Timeout timeoutTimer;
    
    int data = 10;
    bool readSuccessfull = false;
    
    void signalFall();
    void timeout();
    
public:
    HCSR04();
    HCSR04(PinName _trigPin, PinName _echoPin);
    HCSR04(PinName pin);
    void init(PinName _trigPin, PinName _echoPin);
    void init(PinName pin);
    void startMeasure();
    void sendSignal();
    void stopSignal();
    int read();
    
    void readCallBack(int (*func)(void *obj, int value), void *_arg);
    
};

#endif /* HCSR04_h */
