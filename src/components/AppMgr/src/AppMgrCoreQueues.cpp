#include "AppMgr/AppMgrCoreQueues.h"
#include "LoggerHelper.hpp"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/ALRPCMessage.h"
#include "AppMgr/AppMgrCore.h"

namespace NsAppManager
{

template< class QueueType >
log4cplus::Logger AppMgrCoreQueue<QueueType>::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));

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

template< class QueueType >
AppMgrCoreQueue<QueueType>::AppMgrCoreQueue(const AppMgrCoreQueue &)
    :mThread(0)
    ,mCallbackFn(0)
{
}

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

template< class QueueType >
void AppMgrCoreQueue<QueueType>::executeThreads()
{
    LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");
    mThread->Start(false);

    LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
}

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

template< class QueueType >
void *AppMgrCoreQueue<QueueType>::handleQueue(void *pThis)
{
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
template class AppMgrCoreQueue<ALRPCMessage*>;

}
