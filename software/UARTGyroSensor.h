
#ifndef UARTGyroSensor_h
#define UARTGyroSensor_h

#include "mbed.h"

#include <RunLoop.h>

class UARTGyroSensorDelegate {
public:
    virtual void UARTGyroSensor_valueUpdated() = 0;
};

class UARTGyroSensor : public CallBack {
    
    UARTGyroSensorDelegate *_delegate;
    mbed::Serial *gyro = nullptr;
    
    int8_t value = 0;
    int8_t offset = 0;
    
    void serialCallBack();
    
public:
    
    UARTGyroSensor();
    UARTGyroSensor(UARTGyroSensorDelegate *delegate, PinName tx, PinName rx);
    void init(UARTGyroSensorDelegate *delegate, PinName tx, PinName rx);
    
    void setOffset(int offsetAsDegree);
    
    int8_t getValue();
    float getValueAsFloat();
    
    void runLoopCallBack(void *arg);

    ~UARTGyroSensor();
};

#endif /* UARTGyroSensor_h */
