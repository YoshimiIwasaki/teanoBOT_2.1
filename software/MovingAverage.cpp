
#include "MovingAverage.h"

#include <inttypes.h>

template <typename T>
MovingAverage<T>::MovingAverage(int times) : _ringBuffer(new T[times]), _result(0), _times(times), _count(0)
{
    /* initialize ring buffer */
    for (int i = 0; i < times; i++) _ringBuffer[i] = 0;
}

template <typename T>
T MovingAverage<T>::operator() (T value)
{
    return smooth(value);
}

template <typename T>
MovingAverage<T>::~MovingAverage()
{
    /* deinitialize ring buffer */
    delete[] _ringBuffer;
}

template <typename T>
T MovingAverage<T>::smooth(T data)
{
    T old, latest;
    old     = _ringBuffer[_count];
    latest  = data;
    
    /* remove old value */
    _result -= (static_cast<float>(old)    / _times);
    /* add latest value */
    _result += (static_cast<float>(latest) / _times);
    
    /* update ring buffer */
    _ringBuffer[_count] = latest;
    
    /* increment count
     if it reaches the maximum value, substitute 0 */
    if (++_count == _times) _count = 0;
    
    return static_cast<T>(_result);
}

template class MovingAverage<int8_t>;
template class MovingAverage<int16_t>;
template class MovingAverage<int32_t>;
template class MovingAverage<int64_t>;

template class MovingAverage<uint8_t>;
template class MovingAverage<uint16_t>;
template class MovingAverage<uint32_t>;
template class MovingAverage<uint64_t>;

template class MovingAverage<float>;
template class MovingAverage<double>;


#include <inttypes.h>
#define _USE_MATH_DEFINES
#include <math.h>

#if defined(__MBED__)
#   include "mbed.h"
//#   define M_PI 3.1415926535897932384626433832795
//#   include "arm_math.h"
//#   include "arm_const_structs.h"
//#   define sin(x) arm_sin_f32(x)
//#   define cos(x) arm_cos_f32(x)
#elif defined(ARDUINO)
#   define M_PI PI
#   undef degrees
#   undef radians
#endif

#   define degrees(x) ((x)*(180.0f)/(static_cast<float>(M_PI)))
#   define radians(x) ((x)*(static_cast<float>(M_PI))/(180.0f))


template <typename T>
AnglesMovingAverage<T>::AnglesMovingAverage(int times, bool useRadian)
: xComponentAverage(times), yComponentAverage(times), _useRadian(useRadian)
{
    /* do nothing */
}

template <typename T>
T AnglesMovingAverage<T>::operator() (T value)
{
    return smooth(value);
}

template <typename T>
AnglesMovingAverage<T>::~AnglesMovingAverage()
{
    /* do nothing */
}

template <typename T>
T AnglesMovingAverage<T>::smooth(T angle)
{
    float x, y;
    x = xComponentAverage( cos(_useRadian ? angle : radians(angle)) );
    y = yComponentAverage( sin(_useRadian ? angle : radians(angle)) );
    
    return static_cast<T>(_useRadian ? atan2(y, x) : degrees( atan2(y, x) ));
}

template class AnglesMovingAverage<int8_t>;
template class AnglesMovingAverage<int16_t>;
template class AnglesMovingAverage<int32_t>;
template class AnglesMovingAverage<int64_t>;

template class AnglesMovingAverage<uint8_t>;
template class AnglesMovingAverage<uint16_t>;
template class AnglesMovingAverage<uint32_t>;
template class AnglesMovingAverage<uint64_t>;

template class AnglesMovingAverage<float>;
template class AnglesMovingAverage<double>;

