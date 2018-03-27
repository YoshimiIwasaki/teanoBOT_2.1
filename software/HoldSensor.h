
#ifndef HoldSensor_h
#define HoldSensor_h

#include "mbed.h"

#include "RunLoop.h"

class HoldSensorDelegate {
public:
    virtual void HoldSensor_ballDetected() = 0;
};

class HoldSensor : public CallBack {
    
    HoldSensorDelegate *delegate;
    
    mbed::InterruptIn *pin = nullptr;
    
    bool alreadyDispatched = false;
    
    void riseCallBack();
    void runLoopCallBack(void *arg);
    
public:
    
    HoldSensor();
    HoldSensor(HoldSensorDelegate *_delegate, PinName _pin);
    void init(HoldSensorDelegate *_delegate, PinName _pin);
    bool read();
    operator bool();
    ~HoldSensor();
    
};

#endif /* HoldSensor_h */
