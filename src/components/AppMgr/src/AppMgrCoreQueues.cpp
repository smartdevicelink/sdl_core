//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "AppMgr/AppMgrCoreQueues.h"
#include "LoggerHelper.hpp"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/SDLRPCMessage.h"
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

        if( mQueue.size() > 0 )
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Emptying queue elements");
            mMtx.Lock();
            while( mQueue.size() > 0 )
            {
                mQueue.pop();
            }
            mMtx.Unlock();
            LOG4CPLUS_INFO_EXT(mLogger, "Queue empty.");
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
        if(!mCallbackFn)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " callback function pointer should be non-null!");
            return 0;
        }
        while(true)
        {
            bool result = false;
            while ( mMtx.Lock(), result = !mQueue.empty(), mMtx.Unlock(), result )
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

    template class AppMgrCoreQueue<NsRPC2Communication::RPC2Command*>;
    template class AppMgrCoreQueue<Message>;
    template class AppMgrCoreQueue<NsSmartDeviceLinkRPC::SDLRPCMessage*>;

}
