
#ifndef ToggleSwitch_h
#define ToggleSwitch_h

#include "mbed.h"

#include "RunLoop.h"

class ToggleSwitch;

class ToggleSwitchDelegate {
public:
    virtual void ToggleSwitch_rise(ToggleSwitch *sw) = 0;
    virtual void ToggleSwitch_fall(ToggleSwitch *sw) = 0;
};

class ToggleSwitch : public CallBack {
    ToggleSwitchDelegate *delegate;
    
    mbed::InterruptIn *toggle;
    bool reversed;
    
    mbed::Timeout chatteringTimer;
    bool changeDetected = false;
    
    void stateChangeDetected();
    void refresh();
    
public:
    ToggleSwitch();
    ToggleSwitch(ToggleSwitchDelegate *_delegate, PinName pin, bool _reversed = false);
    void init(ToggleSwitchDelegate *_delegate, PinName pin, bool _reversed = false);
    
    void runLoopCallBack(void *arg);

    ~ToggleSwitch();
    
};

#endif /* ToggleSwitch_h */
