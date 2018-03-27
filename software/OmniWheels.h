
#ifndef OmniWheels_h
#define OmniWheels_h

#include "mbed.h"
#include "rtos.h"

#include "FullBridgeMotorDriver.h"

#include "RunLoop.h"
#include "SharedObjects.h"

typedef enum {
    movePriorityAboveHigh =  3,
    movePriorityHigh        =  2,
    movePriorityAboveNormal =  1,
    movePriorityNormal      =  0,
    movePriorityBelowNormal = -1,
    movePriorityLow         = -2
} movePriority;

typedef enum {
    vector_none,
    vector_x_none,
    vector_y_none,
    vector_x_plus,
    vector_x_minus,
    vector_y_plus,
    vector_y_minus
} vectorCancelDirection;

typedef struct {
    int direction;
    float speed;
    movePriority priority;
} moveInfo;

class Motor {
public:
    
    virtual void spin(float speedAndDirection) = 0;
    virtual void brake() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    
    void release() {
        spin(0);
    }
    
    Motor& operator= (float value) {
        spin(value);
        return *this;
    }
    
};

class OmniWheelsDelegate {
public:
    virtual void OmniWheels_moveWithoutTimeout() = 0;
};

class OmniWheels : public CallBack {
    OmniWheelsDelegate *delegate;
    FullBridgeMotorDriver *motors;
    int *degreeOfMotors;
    int numOfMotor;
    float fixValue = 0.0;
    
    FullBridgeMotorDriver *motorPointerLimit;
    
    moveInfo nextMoveInfo;
    
    vectorCancelDirection vectorCancelX = vector_none;
    vectorCancelDirection vectorCancelY = vector_none;
    
    mbed::Timeout vectorCancelResetTimerX;
    mbed::Timeout vectorCancelResetTimerY;
    mbed::Ticker runLoopSetTimer;
    
    void resetVectorCancelX();
    void resetVectorCancelY();
    void dispatchMySelf();
    
    inline static constexpr float degreeToRadian(int degree) {
        return static_cast<float>(degree) * static_cast<float>(M_PI) / 180.0f;
    }
    
    inline static constexpr int   radianToDegree(float radian) {
        return radian * 180.0f / static_cast<float>(M_PI) ;
    }
    
public:

    OmniWheels(
               OmniWheelsDelegate *_delegate,
               int   _numOfMotor,
               FullBridgeMotorDriver *_motors,
               int   *_degreeOfMotors,
               int   _frequency_ms
              );
    
    
    void setFixValue(float _fixValue);
    void setMoveInfo(moveInfo *mvInfo);
    void setVectorCancel(vectorCancelDirection dir, int timeout_ms = 0);
    void setCancelDirection(Cupteano::Direction dir, int timeout_ms = 0);
    
    vectorCancelDirection getVCX();
    vectorCancelDirection getVCY();
    
    void brake();
    
    void runLoopCallBack(void *arg);
    
    void enable();
    void disable();
    
    
};


#endif /* OmniWheels_h */
