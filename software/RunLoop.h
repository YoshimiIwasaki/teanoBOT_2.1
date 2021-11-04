
#ifndef RunLoop_h
#define RunLoop_h

#include "mbed.h"
#include "rtos.h"

#include <queue>

class CallBack {
public:
    virtual void runLoopCallBack(void *arg) = 0;
};

class RunLoop {
private:
    class FunctionWrapper {
        CallBack *obj;
        void *argument;
    public:
        FunctionWrapper(CallBack *_obj, void *_argument): obj(_obj), argument(_argument) {}

        void operator()(void) {
            obj->runLoopCallBack(argument);
        }
    };

    typedef FunctionWrapper runloop_t;
    static constexpr int queueSize = 15;
    Mail<runloop_t, queueSize> queue;

    bool isRunning = false;
    
    RunLoop();
    
public:
    static RunLoop mainLoop;
    void __attribute__((noreturn)) run();
    void dispatch(CallBack *obj, void *argument);
};

#endif /* RunLoop_h */
