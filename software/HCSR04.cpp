
#include "HCSR04.h"

extern mbed::Serial pc;

HCSR04::HCSR04()
{
    
}

HCSR04::HCSR04(PinName _trigPin, PinName _echoPin)
{
    init(_trigPin, _echoPin);
}

HCSR04::HCSR04(PinName pin)
{
    init(pin);
}

void HCSR04::init(PinName _trigPin, PinName _echoPin)
{
    trigPin = _trigPin;
    echoPin = _echoPin;
    trig = new mbed::DigitalOut(trigPin);
    echo = new mbed::InterruptIn(echoPin);
    echo->fall(mbed::callback(this, &HCSR04::signalFall));
}

void HCSR04::init(PinName pin)
{
    trigPin = pin;
    echoPin = pin;
}

void HCSR04::sendSignal()
{
    if (trig == nullptr) {
        if (echo != nullptr) {
            delete echo;
            echo = nullptr;
        }
        trig = new mbed::DigitalOut(trigPin);
    }
    
    trig->write(1);
}

void HCSR04::stopSignal()
{
    if (trig == nullptr) {
        if (echo != nullptr) {
            delete echo;
            echo = nullptr;
        }
        trig = new mbed::DigitalOut(trigPin);
    }
    
    trig->write(0);
    
    if (echo == nullptr) {
        if (trig != nullptr) {
            delete trig;
            trig = nullptr;
        }
        echo = new mbed::InterruptIn(echoPin);
        echo->fall(mbed::callback(this, &HCSR04::signalFall));
    }
    
    timer.start();
    
//    timeoutTimer.attach_us(mbed::callback(this, &HCSR04::timeout), 55*1000);
        timeoutTimer.attach_us(mbed::callback(this, &HCSR04::timeout), 10*1000);
    
    readSuccessfull = false;
}

void HCSR04::startMeasure()
{
    sendSignal();
    wait_us(10);
    stopSignal();
}

int HCSR04::read()
{
    return data;
}


void HCSR04::signalFall()
{
    readSuccessfull = true;
    
    data = static_cast<int>(( static_cast<float>(timer.read_us()) / 2.0f) * 340.0f * 100.0f / 1000000.0f);
    data -= 30; if (data < 0) data = 0;
    
    timer.stop();
    timer.reset();
    
    timeoutTimer.detach();
    
}

void HCSR04::timeout()
{
    if (readSuccessfull) return;
    
    data = 0;
    
    timer.stop();
    timer.reset();
}

void HCSR04::readCallBack(int (*func)(void *obj, int value), void *_arg)
{
    
}
