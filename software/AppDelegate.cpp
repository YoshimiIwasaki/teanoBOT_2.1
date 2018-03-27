
#include "AppDelegate.h"
#include "SharedObjects.h"

AppDelegate::AppDelegate()
{
    init();
}

void AppDelegate::init()
{
    
    gyro = new UARTGyroSensor;
    gyro->init(this, PC_12, PD_2);
    
    ir = new UARTIRSensor;
    ir->init(this, PC_10, PC_11);
    
    sw1 = new ToggleSwitch;
    sw1->init(this, PA_12);
    
    motors = new FullBridgeMotorDriver[numOfMotor];
    motors[0].init(PC_9, PC_8, PA_0);
    motors[1].init(PC_5, PC_4, PA_1);
    motors[2].init(PC_3, PC_2, PB_4);
    degreeOfMotor = new int[numOfMotor];
    degreeOfMotor[0] = 180;
    degreeOfMotor[1] = 30;
    degreeOfMotor[2] = -30;

    omni = new OmniWheels(this, numOfMotor, motors, degreeOfMotor, 10);
}

void AppDelegate::OmniWheels_moveWithoutTimeout()
{

}

void AppDelegate::UARTGyroSensor_valueUpdated()
{
    omni->setFixValue(pid.pid(gyro->getValueAsFloat()));
}

void AppDelegate::UARTIRSensor_valueUpdated(Cupteano::BallState bs) {
    
    moveInfo mvinfo;

    mvinfo.speed = 0.5;
    mvinfo.priority = movePriorityNormal;

    int angle = ir->getDirection();
    int distance = ir->getDistance();


    if (bs == Cupteano::BallState::Near) {
        if (fabs(angle) < 30) {
            mvinfo.direction = angle;
        } else if (fabs(angle) > 90) {
            mvinfo.direction = angle > 0 ? angle + 65 : angle - 65;
        } else {
            mvinfo.direction = angle > 0 ? angle + 90 : angle - 90;
        }
    } else {
        mvinfo.direction = angle;
    }

    omni->setMoveInfo(&mvinfo);
}

void AppDelegate::HoldSensor_ballDetected()
{

    kicker.kick();
}

void AppDelegate::CrossLineSensor_lineDetected(Cupteano::Direction ld)
{
    using namespace Cupteano;
    
    moveInfo mvinfo;
    mvinfo.direction = reverseAngles( directionToAngle(ld) );
    mvinfo.speed = 0.99;
    mvinfo.priority = movePriorityHigh;

    omni->setMoveInfo(&mvinfo);
}

void AppDelegate::CrossLineSensor_escapedFromLine(Cupteano::Direction ld)
{
    moveInfo mvinfo;
    mvinfo.direction = 0;
    mvinfo.speed = 0;
    mvinfo.priority = movePriorityAboveHigh;
    omni->setMoveInfo(&mvinfo);

    omni->setCancelDirection(ld);
}

void AppDelegate::ToggleSwitch_rise(ToggleSwitch *sw)
{
    if (sw == sw1) {
        omni->enable();
        omni->setCancelDirection(Cupteano::Direction::None);
    }
}

void AppDelegate::ToggleSwitch_fall(ToggleSwitch *sw)
{
    if (sw == sw1) {
        omni->disable();
        omni->setCancelDirection(Cupteano::Direction::None);
    }
}
