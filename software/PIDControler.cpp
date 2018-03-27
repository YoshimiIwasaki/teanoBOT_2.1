
#include "PIDControler.h"

template <typename T>
PIDControler<T>::PIDControler(float _Kp, float _Ki, float _Kd, float _deadZone) : Kp(_Kp), Ki(_Ki), Kd(_Kd), deadZone(_deadZone)
{
    differentialTimer.start();
}

template <typename T>
T PIDControler<T>::pid(T deviation)
{
    float deltaT = differentialTimer.read_ms() / 1000.0f;
    differentialTimer.stop();
    differentialTimer.reset();
    
    float differential = static_cast<float>(deviation - previousDeviation) / deltaT;

    previousDeviation = deviation;
    differentialTimer.start();
    
    if (fabs(deviation) < deadZone) return 0;
    return static_cast<T>(Kp * deviation + Ki * 0 + Kd * differential);
    
}

template class PIDControler<int>;
template class PIDControler<float>;
