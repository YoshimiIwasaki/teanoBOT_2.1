
#ifndef UARTIRSensor_h
#define UARTIRSensor_h

#include "mbed.h"
#include "rtos.h"

#include "RunLoop.h"
#include "SharedObjects.h"

class UARTIRSensorDelegate {
public:
    virtual void UARTIRSensor_valueUpdated(Cupteano::BallState bs) = 0;
};

class UARTIRSensor : public CallBack {
    
    UARTIRSensorDelegate *delegate = nullptr;
    
    mbed::RawSerial *ir = nullptr;
    
    uint8_t directionLowerBit = 0;
    uint8_t directionUpperBit = 0;
    uint8_t distanceBit       = 0;
    
    enum BitType {
        dirLowerBit = 0,
        dirUpperBit = 1,
        disBit      = 2,
    };
    
    Cupteano::BallState currentBallState = Cupteano::BallState::None;
    
    void serialCallBack();
    
public:
    
    UARTIRSensor();
    UARTIRSensor(UARTIRSensorDelegate *_delegate, PinName tx, PinName rx);
    void init(UARTIRSensorDelegate *_delegate, PinName tx, PinName rx);
    
    int getDirection();
    int getDistance();
    
    Cupteano::BallState getBallState();
    
    
    void runLoopCallBack(void *arg);
    
    ~UARTIRSensor();
};

#endif /* UARTIRSensor_h */
