
#include "FullBridgeMotorDriver.h"

FullBridgeMotorDriver::FullBridgeMotorDriver()
{
    
}

FullBridgeMotorDriver::FullBridgeMotorDriver(PinName _pinA, PinName _pinB, PinName _pinP)
{
    init(_pinA, _pinB, _pinP);
}

void FullBridgeMotorDriver::init(PinName _pinA, PinName _pinB, PinName _pinP)
{
    if (pinA != nullptr) delete pinA;
    if (pinB != nullptr) delete pinB;
    if (pinP != nullptr) delete pinP;
    
    pinA = new mbed::DigitalOut(_pinA);
    pinB = new mbed::DigitalOut(_pinB);
    pinP = new mbed::PwmOut(_pinP);
    
    pinP->period_us(50); // 20 kHz
    
    enable();
}

void FullBridgeMotorDriver::spin(float speedAndDirection)
{
    if (! isEnable || pinP == nullptr) return;
    
    if (speedAndDirection > 0) {
        pinA->write(1);
        pinB->write(0);
    } else if (speedAndDirection < 0) {
        pinA->write(0);
        pinB->write(1);
    } else {
        pinA->write(0);
        pinB->write(0);
    }
    
    if (speedAndDirection > 0) {
        pinP->write(speedAndDirection< 1.0f ?  speedAndDirection : 1.0f);
    } else {
        pinP->write(speedAndDirection>-1.0f ? -speedAndDirection : 1.0f);
    }
}

void FullBridgeMotorDriver::brake()
{
    pinA->write(1);
    pinB->write(1);
    pinP->write(0);
}

void FullBridgeMotorDriver::enable()
{
    isEnable = true;
}

void FullBridgeMotorDriver::disable()
{
    brake();
    isEnable = false;
}

FullBridgeMotorDriver::~FullBridgeMotorDriver()
{
    if (pinA != nullptr) delete pinA;
    if (pinB != nullptr) delete pinB;
    if (pinP != nullptr) delete pinP;
}
