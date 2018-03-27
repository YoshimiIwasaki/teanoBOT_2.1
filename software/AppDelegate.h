
#ifndef AppDelegate_h
#define AppDelegate_h

#include "mbed.h"
#include "rtos.h"

#include "OmniWheels.h"
#include "FullBridgeMotorDriver.h"
#include "UARTGyroSensor.h"
#include "UARTIRSensor.h"
#include "ToggleSwitch.h"
#include "HoldSensor.h"
#include "Kicker.h"
#include "CrossLineSensor.h"
#include "HCSR04.h"

#include "PIDControler.h"

class AppDelegate : public OmniWheelsDelegate , public UARTGyroSensorDelegate , public ToggleSwitchDelegate, public UARTIRSensorDelegate, public HoldSensorDelegate, public CrossLineSensorDelegate {
    
    FullBridgeMotorDriver *motors;
    OmniWheels            *omni;
    
    UARTGyroSensor        *gyro;
    UARTIRSensor          *ir;
    
    
    ToggleSwitch *sw1;

    PIDControler<float> pid = PIDControler<float>(5.5, 0.0, 0.3, 0.002);
    
    PinName parallel[6] = {PB_15, PB_14, PB_13, PB_9, PB_8, PB_7 };
    CrossLineSensor line = CrossLineSensor(this, parallel[0], parallel[1], parallel[3], parallel[2]);
    HoldSensor hold = HoldSensor(this, parallel[4]);
    
    Kicker kicker = Kicker(PC_0);
    
    int numOfMotor = 3;
    int *degreeOfMotor;
   
    void init();
    void OmniWheels_moveWithoutTimeout();
    void UARTGyroSensor_valueUpdated();
    void UARTIRSensor_valueUpdated(Cupteano::BallState bs);
    
    void ToggleSwitch_rise(ToggleSwitch *sw);
    void ToggleSwitch_fall(ToggleSwitch *sw);
    
    void HoldSensor_ballDetected();
    
    void CrossLineSensor_lineDetected(Cupteano::Direction ld);
    void CrossLineSensor_escapedFromLine(Cupteano::Direction ld);
    
public:
    AppDelegate();

};


#endif /* AppDelegate_h */
