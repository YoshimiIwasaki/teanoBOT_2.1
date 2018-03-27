
#ifndef CrossLineSensor_h
#define CrossLineSensor_h

#include "mbed.h"
#include "rtos.h"

#include "RunLoop.h"
#include "SharedObjects.h"


class CrossLineSensorDelegate {
public:
    virtual void CrossLineSensor_lineDetected(Cupteano::Direction ld) = 0;
    virtual void CrossLineSensor_escapedFromLine(Cupteano::Direction ld) = 0;
};

class CrossLineSensor : public CallBack {
    
    CrossLineSensorDelegate *delegate;
    
    mbed::InterruptIn *left  = nullptr;
    mbed::InterruptIn *right = nullptr;
    mbed::InterruptIn *back  = nullptr;
    mbed::InterruptIn *front = nullptr;

    uint64_t lineDetectedTimes = 0;
    uint64_t leftTimes = 0;
    uint64_t rightTimes = 0;
    uint64_t backTimes = 0;
    uint64_t frontTimes = 0;
    
    bool alreadyDispatched = false;
    
    void lineDetected();
    
    Cupteano::Direction getDirection();
    
public:
  
    CrossLineSensor();
    CrossLineSensor(CrossLineSensorDelegate *_delegate,
                    PinName leftPin, PinName rightPin, PinName backPin, PinName frontPin);
    void init(CrossLineSensorDelegate *_delegate,
              PinName leftPin, PinName rightPin, PinName backPin, PinName frontPin);
    void runLoopCallBack(void *arg);
    ~CrossLineSensor();
    
};


#endif /* CrossLineSensor_h */
