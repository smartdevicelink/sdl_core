#include "AppMgr/AppMgrCoreQueues.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

template< class QueueType >
log4cplus::Logger AppMgrCoreQueues<QueueType>::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));

template< class QueueType >
AppMgrCoreQueues<QueueType>::AppMgrCoreQueues(HandlerCallback *cbFn)
    :mThread(new System::ThreadArgImpl<AppMgrCoreQueues>(*this, &AppMgrCoreQueues::handleQueue, NULL))
    ,mCallbackFn(cbFn)
{
    LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCoreQueues constructed!");
}

template< class QueueType >
AppMgrCoreQueues<QueueType>::AppMgrCoreQueues(const AppMgrCoreQueues &)
    :mThread(0)
    ,mCallbackFn(0)
{
}

template< class QueueType >
AppMgrCoreQueues<QueueType>::~AppMgrCoreQueues()
{
    if(!mThread.Join())
        mThread.Stop();

    LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCoreQueues detructed!");
}

template< class QueueType >
void AppMgrCoreQueues<QueueType>::executeThreads()
{
    LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");
    mThread.Start(false);

    LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
}

template< class QueueType >
void AppMgrCoreQueues<QueueType>::pushMessage( QueueType message )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Pushing a message...");
    mMtx.Lock();

    mQueue.push(message);

    mMtx.Unlock();
    LOG4CPLUS_INFO_EXT(mLogger, " Pushed a message");
}

template< class QueueType >
void *AppMgrCoreQueues<QueueType>::handleQueue(void *)
{
    while(true)
    {
        std::size_t size = mQueue.size();
        if( size > 0 )
        {
            mMtx.Lock();
            QueueType msg = mQueue.front();
            mQueue.pop();
            mMtx.Unlock();

            mCallbackFn( msg );
        }
    }
}

}
