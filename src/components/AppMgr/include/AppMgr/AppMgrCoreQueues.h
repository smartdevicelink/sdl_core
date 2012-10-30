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

namespace NsAppManager
{

template< class QueueType >
class AppMgrCoreQueues
{
public:
    typedef void (*HandlerCallback)( QueueType );

    AppMgrCoreQueues(HandlerCallback* cbFn);

    void executeThreads();
    void pushMessage(QueueType message);

private:
    AppMgrCoreQueues( const AppMgrCoreQueues& );
    virtual ~AppMgrCoreQueues();
    void handleMessage( QueueType message );
    void* handleQueue( void* );

    std::queue< QueueType > mQueue;
    System::Mutex mMtx;
    System::Thread mThread;
    HandlerCallback mCallbackFn;
    static log4cplus::Logger mLogger;
};

}

#endif // APPMGRCOREQUEUES_H
