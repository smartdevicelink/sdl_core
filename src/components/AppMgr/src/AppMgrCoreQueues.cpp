#include "AppMgr/AppMgrCoreQueues.h"
#include "LoggerHelper.hpp"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/ALRPCMessage.h"
#include "AppMgr/AppMgrCore.h"

namespace NsAppManager
{

template< class QueueType >
log4cplus::Logger AppMgrCoreQueue<QueueType>::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));

/**
 * \brief Class constructor
 * \param cbFn callback function for a handler thread
 * \param pThis pointer to an instance of the class the cbFn is being called of
 */
template< class QueueType >
AppMgrCoreQueue<QueueType>::AppMgrCoreQueue(HandlerCallback cbFn, void *pThis)
    :mThread(new System::Thread(new System::ThreadArgImpl<AppMgrCoreQueue>(*this, &AppMgrCoreQueue::handleQueue, pThis)))
    ,mCallbackFn((HandlerCallback)cbFn)
{
    if(pThis && mThread && mCallbackFn)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCoreQueues constructed!");
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " AppMgrCoreQueues not constructed! Check if params are non-null!");
    }
}

/**
 * \brief Default copy constructor
 */
template< class QueueType >
AppMgrCoreQueue<QueueType>::AppMgrCoreQueue(const AppMgrCoreQueue &)
    :mThread(0)
    ,mCallbackFn(0)
{
}

/**
 * \brief Default destructor
 */
template< class QueueType >
AppMgrCoreQueue<QueueType>::~AppMgrCoreQueue()
{
    if(mThread)
    {
        if(!mThread->Join())
            mThread->Stop();
        delete mThread;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCoreQueues detructed!");
}

/**
 * \brief execute inner threads
 */
template< class QueueType >
void AppMgrCoreQueue<QueueType>::executeThreads()
{
    LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");
    mThread->Start(false);

    LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
}

/**
 * \brief push a message to a queue
 * \param message a message being pushed
 */
template< class QueueType >
void AppMgrCoreQueue<QueueType>::pushMessage( QueueType message )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Pushing a message...");
    mMtx.Lock();

    mQueue.push(message);

    mMtx.Unlock();

    mBinarySemaphore.Notify();
    LOG4CPLUS_INFO_EXT(mLogger, " Pushed a message");
}

/**
 * \brief handle a queue
 * \param pThis a pointer to an instance of the class the callback function is being called of
 */
template< class QueueType >
void *AppMgrCoreQueue<QueueType>::handleQueue(void *pThis)
{
    if(!pThis)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " pThis should be non-null!");
        return 0;
    }
    while(true)
    {
        std::size_t size = mQueue.size();
        if( size > 0 )
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Handling message in queue.");
            mMtx.Lock();
            QueueType msg = mQueue.front();
            mQueue.pop();
            mMtx.Unlock();

            mCallbackFn( msg, pThis );
            LOG4CPLUS_INFO_EXT(mLogger, "Message handled.");
        }
        mBinarySemaphore.Wait();
    }
}

template class AppMgrCoreQueue<RPC2Communication::RPC2Command*>;
template class AppMgrCoreQueue<Message>;
template class AppMgrCoreQueue<NsAppLinkRPC::ALRPCMessage*>;

}
