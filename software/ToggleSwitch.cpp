
#include "ToggleSwitch.h"

ToggleSwitch::ToggleSwitch()
{
    
}

ToggleSwitch::ToggleSwitch(ToggleSwitchDelegate *_delegate, PinName pin, bool _reversed)
{
    init(_delegate, pin, _reversed);
}

void ToggleSwitch::init(ToggleSwitchDelegate *_delegate, PinName pin, bool _reversed)
{
    delegate = _delegate;
    toggle = new mbed::InterruptIn(pin);
    reversed = _reversed;
    
    toggle->mode(PullDown);
    
    toggle->rise(mbed::callback(this, &ToggleSwitch::stateChangeDetected));
    toggle->fall(mbed::callback(this, &ToggleSwitch::stateChangeDetected));
    
    refresh();
}

void ToggleSwitch::stateChangeDetected()
{
    if (! changeDetected) {
        changeDetected = true;
        chatteringTimer.attach_us(mbed::callback(this, &ToggleSwitch::refresh), 1000);
    }
}

void ToggleSwitch::refresh()
{
    RunLoop::mainLoop.dispatch(this, reinterpret_cast<void *>(reversed ? !(toggle->read()) : toggle->read()));
    changeDetected = false;
}

void ToggleSwitch::runLoopCallBack(void *arg)
{
    if (reinterpret_cast<int>(arg)) {
        delegate->ToggleSwitch_rise(this);
    } else {
        delegate->ToggleSwitch_fall(this);
    }
}
