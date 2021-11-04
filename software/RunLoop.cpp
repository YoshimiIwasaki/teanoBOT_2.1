
#include "RunLoop.h"

RunLoop RunLoop::mainLoop = RunLoop();

RunLoop::RunLoop(){}

void RunLoop::run(){
    if (isRunning) error("called twice\n");
    while (true) {
        isRunning = true;
        osEvent event = queue.get();
        if (event.status == osEventMail) {
            runloop_t *func = static_cast<runloop_t*>(event.value.p);
            (*func)();
            queue.free(func);
        }
    }
}

void RunLoop::dispatch(CallBack *obj, void *argument){
    runloop_t *func = queue.alloc();
    *func = FunctionWrapper(obj, argument);
    queue.put(func);
}
