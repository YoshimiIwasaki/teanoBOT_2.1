
#ifndef Kicker_h
#define Kicker_h

#include "mbed.h"
#include "rtos.h"

class Kicker {

    mbed::DigitalOut *kicker = nullptr;
    
    static constexpr int timeUntilUnkick = 30;
    static constexpr int timeUntilKickerReturn = 100;
    static constexpr int timeUntilAllowNextKick = 3000;
    
    bool isEnable = false;
    
    bool isNextKickAllow = true;
    
    void unkick();
    void allowNextKick();
    
    mbed::Timeout unkickTimer;
    mbed::Timeout allowNextKickTimer;
    
public:
    
    Kicker();
    Kicker(PinName pin);
    void init(PinName pin);
    void enable();
    void disable();
    void kick();
};

#endif /* Kicker_h */
