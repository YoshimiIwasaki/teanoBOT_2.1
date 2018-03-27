
#include "HoldSensor.h"

HoldSensor::HoldSensor()
{
    
}

HoldSensor::HoldSensor(HoldSensorDelegate *_delegate, PinName _pin)
{
    init(_delegate, _pin);
}

void HoldSensor::init(HoldSensorDelegate *_delegate, PinName _pin)
{
    if (pin != nullptr) delete pin;
    
    delegate = _delegate;
    pin = new mbed::InterruptIn(_pin);
    pin->rise(mbed::callback(this, &HoldSensor::riseCallBack));
}

bool HoldSensor::read()
{
    return pin->read();
}

HoldSensor::operator bool()
{
    return read();
}

void HoldSensor::riseCallBack()
{
    if (! alreadyDispatched) {
        RunLoop::mainLoop.dispatch(this, nullptr);
        alreadyDispatched = true;
    }
}

void HoldSensor::runLoopCallBack(void __attribute__((unused)) *arg)
{
    if (pin->read()) {
        delegate->HoldSensor_ballDetected();
        RunLoop::mainLoop.dispatch(this, nullptr);
    } else {
        alreadyDispatched = false;
    }
}
