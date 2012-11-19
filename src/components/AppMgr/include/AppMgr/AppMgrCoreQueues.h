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

namespace NsRPC2Communication
{
    class RPC2Command;
}

namespace NsAppManager
{

/**
 * \brief AppMgrCore queue acts as a container for various messages that need to be parsed/handled in different threads
 * \class QueueType type of queue elements
 */
template< class QueueType >
class AppMgrCoreQueue
{
public:
    /**
     * \brief Callback function
     * \param QueueType type of queue elements
     * \param pThis pointer to an instance of the class the callback is being called of
     */
    typedef void (*HandlerCallback)( QueueType, void* );

    /**
     * \brief Class constructor
     * \param cbFn callback function for a handler thread
     * \param pThis pointer to an instance of the class the cbFn is being called of
     */
    AppMgrCoreQueue(HandlerCallback cbFn, void *pThis);

    /**
     * \brief Default destructor
     */
    virtual ~AppMgrCoreQueue();

    /**
     * \brief execute inner threads
     */
    void executeThreads();

    /**
     * \brief push a message to a queue
     * \param message a message being pushed
     */
    void pushMessage(QueueType message);

private:

    /**
     * \brief Default copy constructor
     */
    AppMgrCoreQueue( const AppMgrCoreQueue& );

    /**
     * \brief handle a queue
     * \param pThis a pointer to an instance of the class the callback function is being called of
     */
    void* handleQueue(void* pThis);

    std::queue< QueueType > mQueue;
    System::Mutex mMtx;
    System::BinarySemaphore mBinarySemaphore;
    System::Thread* mThread;
    HandlerCallback mCallbackFn;
    static log4cplus::Logger mLogger;
};

}

#endif // APPMGRCOREQUEUES_H
