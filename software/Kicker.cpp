
#include "Kicker.h"

Kicker::Kicker()
{
    
}

Kicker::Kicker(PinName pin)
{
    init(pin);
}

void Kicker::init(PinName pin)
{
    if (kicker != nullptr) delete kicker;
    kicker = new mbed::DigitalOut(pin);
    enable();
}

void Kicker::enable()
{
    isEnable = true;
}

void Kicker::disable()
{
    isEnable = false;
}

void Kicker::kick()
{
    if (isEnable && isNextKickAllow) {
        kicker->write(1);
        isNextKickAllow = false;
        unkickTimer.attach_us(mbed::callback(this, &Kicker::unkick), timeUntilUnkick*1000);
    }
}

void Kicker::unkick()
{
    kicker->write(0);
    allowNextKickTimer.attach_us(mbed::callback(this, &Kicker::allowNextKick), timeUntilAllowNextKick*1000);
}

void Kicker::allowNextKick()
{
    isNextKickAllow = true;
}
