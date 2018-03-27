
#include "FourSonicSensors.h"

FourSonicSensors::FourSonicSensors()
{
    
}

FourSonicSensors::FourSonicSensors(FourSonicSensorsDelegate *_delegate,
                                   UltraSonicSensor *_left,
                                   UltraSonicSensor *_right,
                                   UltraSonicSensor *_back,
                                   UltraSonicSensor *_front)
{
    init(_delegate, _left, _right, _back, _front);
}

void FourSonicSensors::init(FourSonicSensorsDelegate *_delegate,
                            UltraSonicSensor *_left,
                            UltraSonicSensor *_right,
                            UltraSonicSensor *_back,
                            UltraSonicSensor *_front)
{
    delegate = _delegate;
    left = _left;
    right = _right;
    back = _back;
    front = _front;
    
    dispatchTimer.attach_us(mbed::callback(this, &FourSonicSensors::dispatchMySelf), 60*1000);
}

void FourSonicSensors::runLoopCallBack(void __attribute__((unused)) *arg)
{
    if (left  != nullptr) leftData  = left->read();
    if (right != nullptr) rightData = right->read();
    if (back  != nullptr) backData  = back->read();
    if (front != nullptr) frontData = left->read();
    
    if (leftData  != 0 && leftData  < 30) delegate->FourSonicSensors_LeftTooClose();
    if (rightData != 0 && rightData < 30) delegate->FourSonicSensors_RightTooClose();
    if (backData  != 0 && backData  < 10) delegate->FourSonicSensors_BackTooClose();
    if (frontData != 0 && frontData < 30) delegate->FourSonicSensors_FrontTooClose();
    
    if (left  != nullptr) left->sendSignal();
    if (right != nullptr) right->sendSignal();
    if (back  != nullptr) back->sendSignal();
    if (front != nullptr) front->sendSignal();
    
    wait_us(10);
    
    if (left  != nullptr) left->stopSignal();
    if (right != nullptr) right->stopSignal();
    if (back  != nullptr) back->stopSignal();
    if (front != nullptr) front->stopSignal();
}
