
#include "mbed.h"

#include "SharedObjects.h"
#include "AppDelegate.h"
#include "RunLoop.h"

int main(int MBED_UNUSED argc, const char MBED_UNUSED * argv[])
{
    pc.baud(230400);
    AppDelegate delegate;
    RunLoop::mainLoop.run();
    return 0;
}
