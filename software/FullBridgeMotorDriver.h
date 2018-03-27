
#ifndef FullBridgeMotorDriver_h
#define FullBridgeMotorDriver_h

#include "mbed.h"
#include "rtos.h"


class FullBridgeMotorDriver {
    
    mbed::DigitalOut *pinA = nullptr;
    mbed::DigitalOut *pinB = nullptr;
    mbed::PwmOut     *pinP = nullptr;
    
    bool isEnable = false;
    
public:
    
    FullBridgeMotorDriver();
    FullBridgeMotorDriver(PinName _pinA, PinName _pinB, PinName _pinP);
    void init(PinName _pinA, PinName _pinB, PinName _pinP);
    void spin(float speedAndDirection);
    void brake();
    void enable();
    void disable();
    ~FullBridgeMotorDriver();
    
};

#endif /* FullBridgeMotorDriver_h */
