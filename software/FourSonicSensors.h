
#ifndef FourSonicSensors_h
#define FourSonicSensors_h

#include "mbed.h"
#include "rtos.h"

#include "RunLoop.h"

class FourSonicSensorsDelegate {
public:
    virtual void FourSonicSensors_LeftTooClose() = 0;
    virtual void FourSonicSensors_RightTooClose() = 0;
    virtual void FourSonicSensors_BackTooClose() = 0;
    virtual void FourSonicSensors_FrontTooClose() = 0;
};

class UltraSonicSensor {
public:
    virtual void startMeasure() = 0;
    virtual void sendSignal() = 0;
    virtual void stopSignal() = 0;
    virtual int  read() = 0;
    virtual void readCallBack(int (*func)(void *obj, int value), void *_arg) = 0;
};

class FourSonicSensors : public CallBack {
    FourSonicSensorsDelegate *delegate = nullptr;
    
    UltraSonicSensor *left  = nullptr;
    UltraSonicSensor *right = nullptr;
    UltraSonicSensor *back  = nullptr;
    UltraSonicSensor *front = nullptr;

    mbed::Ticker dispatchTimer;
    
    void dispatchMySelf();
    
    int leftData  = 0;
    int rightData = 0;
    int backData  = 0;
    int frontData = 0;
    
public:
    FourSonicSensors();
    FourSonicSensors(FourSonicSensorsDelegate *_delegate,
                     UltraSonicSensor *_left,
                     UltraSonicSensor *_right,
                     UltraSonicSensor *_back,
                     UltraSonicSensor *_front);
    void init(FourSonicSensorsDelegate *_delegate,
              UltraSonicSensor *_left,
              UltraSonicSensor *_right,
              UltraSonicSensor *_back,
              UltraSonicSensor *_front);
    void getPosition(int *x, int *y);
    void getPositionDetails(int *x, int *y);
    void runLoopCallBack(void *arg);
    
};

#endif /* FourSonicSensors_h */
