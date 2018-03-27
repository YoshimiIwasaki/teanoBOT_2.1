
#ifndef PIDControler_h
#define PIDControler_h

#include "mbed.h"

template <typename T>
class PIDControler {
    
    mbed::Timer differentialTimer;
    
    float Kp = 1.0;
    float Ki = 0.0;
    float Kd = 0.0;
    
    float deadZone = 0.0;
    
    T previousDeviation = 0;
    
public:
    
    PIDControler(float _Kp, float _Ki, float _Kd, float _deadZone = 0.0);
    T pid(T deviation);
};


#endif /* PIDControler_h */
