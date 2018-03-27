
#include "OmniWheels.h"

using namespace Cupteano;

static float inflate(float value, float n)
{
    bool sign = value > 0.0 ? 1 : 0; // 1 : plus, 0 : minus
    value *= sign ? 1.0 : -1.0; // absolute value
    value = pow(100*value, 1.0/n) / pow(100, 1.0/n);
    value *= sign ? 1.0 : -1.0; // to origin sign
    return value;
}

OmniWheels::OmniWheels(OmniWheelsDelegate *_delegate, int _numOfMotor, FullBridgeMotorDriver *_motors, int *_degreeOfMotors, int _frequency_ms)
{
    delegate = _delegate;
    numOfMotor = _numOfMotor;
    motors = _motors;
    degreeOfMotors = _degreeOfMotors;
    runLoopSetTimer.attach_us(mbed::callback(this, &OmniWheels::dispatchMySelf), _frequency_ms*1000);
    
    motorPointerLimit = motors + numOfMotor; // points to next address of end of array
    
    enable();
    
    /* Clear move infomation */
    nextMoveInfo.direction = 0;
    nextMoveInfo.speed = 0;
    nextMoveInfo.priority = movePriorityLow;
}


void OmniWheels::setFixValue(float _fixValue)
{
    fixValue = _fixValue;
}

void OmniWheels::setMoveInfo(moveInfo *mvinfo)
{
    if (mvinfo->priority >= nextMoveInfo.priority) {
        
        /* Convert angle from -180 to 179 */
        if (mvinfo->direction == CUPTEANO_ANGLE_NONE || mvinfo->direction == CUPTEANO_ANGLE_ERROR) return;
        int newDirection = mvinfo->direction;
        while (newDirection < -180 || newDirection >= 180) {
            if (newDirection < -180) {
                newDirection += 360;
            } else {
                newDirection -= 360;
            }
        }
        
        /* Set infomation */
        nextMoveInfo.direction   = newDirection;
        nextMoveInfo.speed       = mvinfo->speed;
        nextMoveInfo.priority    = mvinfo->priority;
    }
}

void OmniWheels::setVectorCancel(vectorCancelDirection dir, int timeout_ms)
{
    
    switch (dir) {
            
        case vector_x_plus:
        case vector_x_minus:
            vectorCancelX = dir;
            vectorCancelResetTimerX.detach();
//            pc.printf("VectorCancel X has set\n");
            if (timeout_ms) vectorCancelResetTimerX.attach_us(mbed::callback(this, &OmniWheels::resetVectorCancelX), timeout_ms*1000);
            break;
            
        case vector_y_plus:
        case vector_y_minus:
            vectorCancelY = dir;
            vectorCancelResetTimerY.detach();
//            pc.printf("VectorCancel Y has set\n");
            if (timeout_ms) vectorCancelResetTimerY.attach_us(mbed::callback(this, &OmniWheels::resetVectorCancelY), timeout_ms*1000);
            break;
            
        case vector_none:
            vectorCancelX = dir;
            vectorCancelY = dir;
            break;
            
        case vector_x_none:
            vectorCancelX = vector_none;
            break;
          
        case vector_y_none:
            vectorCancelY = vector_none;
            break;
            
        default:
            break;
        
    }
}

void OmniWheels::setCancelDirection(Cupteano::Direction dir, int timeout_ms)
{
    vectorCancelDirection vdir_x = vector_x_none, vdir_y = vector_y_none;
    switch ( dir ) {
        case Direction::Front:
            vdir_x = vector_x_plus;
            setVectorCancel(vdir_x, timeout_ms);
            return;
        case Direction::Back:
            vdir_x = vector_x_minus;
            setVectorCancel(vdir_x, timeout_ms);
            return;
        case Direction::Left:
            vdir_y = vector_y_minus;
            setVectorCancel(vdir_y, timeout_ms);
            return;
        case Direction::Right:
            vdir_y = vector_y_plus;
            setVectorCancel(vdir_y, timeout_ms);
            return;
        case Direction::LeftFront:
            vdir_y = vector_y_minus;
            vdir_x = vector_x_plus;
            break;
        case Direction::RightFront:
            vdir_y = vector_y_plus;
            vdir_x = vector_x_plus;
            break;
        case Direction::LeftBack:
            vdir_y = vector_y_minus;
            vdir_x = vector_x_minus;
            break;
        case Direction::RightBack:
            vdir_y = vector_y_plus;
            vdir_x = vector_x_minus;
            break;
            
        case Direction::None:
        case Direction::Error:
            break;
            
        default:
            break;
    }
    setVectorCancel(vdir_x, timeout_ms);
    setVectorCancel(vdir_y, timeout_ms);
}

void OmniWheels::runLoopCallBack(void __attribute__((unused)) *arg)
{

    /* Split the vector into two components x and y */
    float moveVectorX = nextMoveInfo.speed * cos( degreeToRadian(nextMoveInfo.direction) );
    float moveVectorY = nextMoveInfo.speed * sin( degreeToRadian(nextMoveInfo.direction) );
    
    /* If vector canceling has set, remove x or y components */
    if        (vectorCancelX == vector_x_plus  && moveVectorX > 0) {
        moveVectorX = 0;
    } else if (vectorCancelX == vector_x_minus && moveVectorX < 0) {
        moveVectorX = 0;
    }
    if        (vectorCancelY == vector_y_plus  && moveVectorY > 0) {
        moveVectorY = 0;
    } else if (vectorCancelY == vector_y_minus && moveVectorY < 0) {
        moveVectorY = 0;
    }
    
    /* Reset vector canceling when necessary */
    if        (vectorCancelX == vector_x_plus  && moveVectorX < -0.01) {
//        pc.printf("reset x plus\n");
        resetVectorCancelX();
    } else if (vectorCancelX == vector_x_minus && moveVectorX > 0.01) {
//        pc.printf("reset x minus\n");
        resetVectorCancelX();
    }
    if        (vectorCancelY == vector_y_plus  && moveVectorY < -0.01) {
//        pc.printf("reset y plus\n");
        resetVectorCancelY();
    } else if (vectorCancelY == vector_y_minus && moveVectorY > 0.01) {
//        pc.printf("reset y minus\n");
        resetVectorCancelY();
    }
    
    /* Combine vectors */
    float newSpeed     = hypot(moveVectorX, moveVectorY);
    int   newDirection = radianToDegree(atan2(moveVectorY, moveVectorX));
    if (nextMoveInfo.priority == movePriorityHigh || nextMoveInfo.priority == movePriorityAboveHigh) {
        newSpeed = nextMoveInfo.speed;
        newDirection = nextMoveInfo.direction;
    }
    
    /* Spin each motors */
    FullBridgeMotorDriver *motorPointer  = motors;
    int   *degreePointer = degreeOfMotors;
    do {
        float value = inflate( newSpeed * -sin( degreeToRadian(newDirection-(*degreePointer)) ), 3.0);
        motorPointer->spin(value + fixValue);
    } while (motorPointer++, degreePointer++, motorPointer != motorPointerLimit);
    
    /* Clear move infomation */
    nextMoveInfo.direction = 0;
    nextMoveInfo.speed = 0;
    nextMoveInfo.priority = movePriorityLow;
    
    /* Call function to test this class */
    delegate->OmniWheels_moveWithoutTimeout();
    
}

void OmniWheels::brake()
{
    /* Enable all motor */
    FullBridgeMotorDriver *motorPointer = motors;
    do {
        motorPointer->brake();
    } while (++motorPointer != motorPointerLimit);
}

void OmniWheels::enable()
{
    /* Enable all motor */
    FullBridgeMotorDriver *motorPointer = motors;
    do {
        motorPointer->enable();
    } while (++motorPointer != motorPointerLimit);
}

void OmniWheels::disable()
{
    /* Disable all motor */
    FullBridgeMotorDriver *motorPointer = motors;
    do {
        motorPointer->disable();
    } while (++motorPointer != motorPointerLimit);
}

void OmniWheels::resetVectorCancelX()
{
    vectorCancelX = vector_none;
}

void OmniWheels::resetVectorCancelY()
{
    vectorCancelY = vector_none;
}

void OmniWheels::dispatchMySelf()
{
    RunLoop::mainLoop.dispatch(this, nullptr);
}

