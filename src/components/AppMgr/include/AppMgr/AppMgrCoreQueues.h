#ifndef APPMGRCOREQUEUES_H
#define APPMGRCOREQUEUES_H

#include "system.h"
#include <queue>
#include <string>
#include <map>

namespace log4cplus
{
    class Logger;
}

namespace RPC2Communication
{
    class RPC2Command;
}

namespace NsAppManager
{

template< class QueueType >
class AppMgrCoreQueue
{
public:
    typedef void (*HandlerCallback)( QueueType, void* );

    AppMgrCoreQueue(HandlerCallback cbFn, void *pThis);
    virtual ~AppMgrCoreQueue();

    void executeThreads();
    void pushMessage(QueueType message);

private:
    AppMgrCoreQueue( const AppMgrCoreQueue& );

    void handleMessage( QueueType message );
    void* handleQueue(void* pThis);

    std::queue< QueueType > mQueue;
    System::Mutex mMtx;
    System::Thread mThread;
    HandlerCallback mCallbackFn;
    static log4cplus::Logger mLogger;
};

}

#endif // APPMGRCOREQUEUES_H
