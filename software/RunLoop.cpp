
#include "RunLoop.h"

RunLoop RunLoop::mainLoop = RunLoop();

RunLoop::RunLoop()
{
//    runloop_t *runLoopPointerBuf = runLoopPointers;
//    do {
//        runLoopPointerAllocQueue.push(runLoopPointerBuf);
//    } while (++runLoopPointerBuf != runLoopPointerLimit);
}

void RunLoop::run()
{
   
//    while (true) {
//        if (! queue.empty()) {
//            runloop_t *func = queue.front();
//            (*func)();
//            *func = FunctionWrapper();
//            queue.pop();
//        }
//    }
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

void RunLoop::dispatch(CallBack *obj, void *argument)
{
//    runloop_t *func = runLoopPointerAllocQueue.front();
//    *func = FunctionWrapper(obj, argument);
//    queue.push(func);
//    runLoopPointerAllocQueue.pop();
//    runLoopPointerAllocQueue.push(func);
    runloop_t *func = queue.alloc();
    *func = FunctionWrapper(obj, argument);
    queue.put(func);
}
