
#include "CrossLineSensor.h"

using namespace Cupteano;

CrossLineSensor::CrossLineSensor()
{
    
}

CrossLineSensor::CrossLineSensor(CrossLineSensorDelegate *_delegate,
                                 PinName leftPin, PinName rightPin, PinName backPin, PinName frontPin)
{
    init(_delegate, leftPin, rightPin, backPin, frontPin);
}

void CrossLineSensor::init(CrossLineSensorDelegate *_delegate,
                                 PinName leftPin, PinName rightPin, PinName backPin, PinName frontPin)
{
    if (left  != nullptr) delete left;
    if (right != nullptr) delete right;
    if (back  != nullptr) delete back;
    if (front != nullptr) delete front;
    
    delegate = _delegate;
    
    left  = new mbed::InterruptIn(leftPin);
    right = new mbed::InterruptIn(rightPin);
    back  = new mbed::InterruptIn(backPin);
    front = new mbed::InterruptIn(frontPin);
    
    left->rise(mbed::callback(this, &CrossLineSensor::lineDetected));
    right->rise(mbed::callback(this, &CrossLineSensor::lineDetected));
    back->rise(mbed::callback(this, &CrossLineSensor::lineDetected));
    front->rise(mbed::callback(this, &CrossLineSensor::lineDetected));
}

void CrossLineSensor::runLoopCallBack(void __attribute__((unused)) *arg)
{
    static Direction previousDirection = Direction::None;
    Direction ld = getDirection();
    
    if ( ld == Direction::None ) {
        alreadyDispatched = false;
        delegate->CrossLineSensor_escapedFromLine(previousDirection);
        previousDirection = Direction::None;
        return;
    } else if ( ld == Direction::Error ) {
        delegate->CrossLineSensor_lineDetected(previousDirection);
    } else {
        delegate->CrossLineSensor_lineDetected(ld);
        previousDirection = ld;
    }
    
    wait_ms(4);
    
    RunLoop::mainLoop.dispatch(this, nullptr);
}

Direction CrossLineSensor::getDirection()
{
    Cupteano::Direction dir;
    uint8_t lineData = front->read() << 3 | back->read() << 2 | left->read() << 1 | right->read() << 0;
    switch (lineData) {
        case 0b0000:
            dir = Direction::None; break;
        case 0b1000:
            dir = Direction::Front; break;
        case 0b0100:
            dir = Direction::Back; break;
        case 0b0010:
            dir = Direction::Left; break;
        case 0b0001:
            dir = Direction::Right; break;
        case 0b1010:
            dir = Direction::LeftFront; break;
        case 0b1001:
            dir = Direction::RightFront; break;
        case 0b0110:
            dir = Direction::LeftBack; break;
        case 0b0101:
            dir = Direction::RightBack; break;
        default:
            dir = Direction::Error; break;
    }
    return dir;
}

void CrossLineSensor::lineDetected()
{
    if (! alreadyDispatched) {
        alreadyDispatched = true;
        RunLoop::mainLoop.dispatch(this, nullptr);
    }
}

CrossLineSensor::~CrossLineSensor()
{
    if (left  != nullptr) delete left;
    if (right != nullptr) delete right;
    if (back  != nullptr) delete back;
    if (front != nullptr) delete front;
}
