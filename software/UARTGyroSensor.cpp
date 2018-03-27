
#include "UARTGyroSensor.h"

UARTGyroSensor::UARTGyroSensor()
{
    
}

UARTGyroSensor::UARTGyroSensor(UARTGyroSensorDelegate *delegate, PinName tx, PinName rx)
{
    init(delegate, tx, rx);
}

void UARTGyroSensor::init(UARTGyroSensorDelegate *delegate, PinName tx, PinName rx)
{
    _delegate = delegate;
    if (gyro != nullptr) delete gyro;
    gyro = new mbed::Serial(tx, rx);
    gyro->baud(115200);
    gyro->attach(mbed::callback(this, &UARTGyroSensor::serialCallBack), mbed::Serial::RxIrq);

}

void UARTGyroSensor::setOffset(int offsetAsDegree)
{
    offset = static_cast<int8_t>(static_cast<float>(offsetAsDegree) * 128.0f / 180.0f);
}

int8_t UARTGyroSensor::getValue()
{
    return value;
}

float UARTGyroSensor::getValueAsFloat()
{
    return value / 128.0f;
}

void UARTGyroSensor::serialCallBack()
{
    value = static_cast<int8_t>(gyro->getc());
    RunLoop::mainLoop.dispatch(this, nullptr);
}

void UARTGyroSensor::runLoopCallBack(void MBED_UNUSED *arg)
{
    _delegate->UARTGyroSensor_valueUpdated();
}

UARTGyroSensor::~UARTGyroSensor()
{
    
}
