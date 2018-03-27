
#include "UARTIRSensor.h"

using namespace Cupteano;

UARTIRSensor::UARTIRSensor()
{
    
}

UARTIRSensor::UARTIRSensor(UARTIRSensorDelegate *_delegate, PinName tx, PinName rx)
{
    init(_delegate, tx, rx);
}

void UARTIRSensor::init(UARTIRSensorDelegate *_delegate, PinName tx, PinName rx)
{
    delegate = _delegate;
    if (ir != nullptr) delete ir;
    ir = new mbed::RawSerial(tx, rx);
    ir->baud(115200);
    ir->attach(mbed::callback(this, &UARTIRSensor::serialCallBack), mbed::Serial::RxIrq);
}

int UARTIRSensor::getDirection()
{
    int direction = (directionUpperBit << 6) | directionLowerBit;
    if (direction < -179) direction += 360;
    else if (direction > 180) direction -= 360;
    return direction;
}

int UARTIRSensor::getDistance()
{
    return distanceBit;
}

BallState UARTIRSensor::getBallState()
{
    return currentBallState;
}

void UARTIRSensor::runLoopCallBack(void MBED_UNUSED *arg)
{
    int distance = getDistance();
    switch (currentBallState) {
        case BallState::None:
            if (distance > 100) {
                currentBallState = BallState::Far;
            } else if (distance > 45) {
                currentBallState = BallState::Middle;
            } else if (distance > 0) {
                currentBallState = BallState::Near;
            }
            break;
        case BallState::Near:
            if (distance > 100) {
                currentBallState = BallState::Far;
            } else if (distance > 45) {
                currentBallState = BallState::Middle;
            } else if (distance == 0) {
                currentBallState = BallState::None;
            }
            break;
        case BallState::Middle:
            if (distance > 100) {
                currentBallState = BallState::Far;
            } else if (distance < 45) {
                currentBallState = BallState::Near;
            } else if (distance == 0) {
                currentBallState = BallState::None;
            }
            break;
        case BallState::Far:
            if (distance < 80) {
                currentBallState = BallState::Middle;
            } else if (distance < 45) {
                currentBallState = BallState::Near;
            } else if (distance == 0) {
                currentBallState = BallState::None;
            }
            break;
            
        default:
            break;
    }
    
    delegate->UARTIRSensor_valueUpdated(currentBallState);
}

void UARTIRSensor::serialCallBack()
{
    uint8_t data = ir->getc();
    
    if ((data >> 6) == 0b00 ) {
        directionLowerBit = data & 0b00111111;
    } else if ((data >> 6) == 0b01 ) {
        directionUpperBit = data & 0b00111111;
    } else {
        distanceBit       = data & 0b01111111;
        if (getDistance() != 0) {
            RunLoop::mainLoop.dispatch(this, nullptr);
        }
    }
}

UARTIRSensor::~UARTIRSensor()
{
    if (ir != nullptr) delete ir;
}

