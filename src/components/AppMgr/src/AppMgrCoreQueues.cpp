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
#include "utils/logger.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/SDLRPCMessage.h"
#include "AppMgr/AppMgrCore.h"

namespace NsAppManager
{

    template< class QueueType >
      log4cxx::LoggerPtr AppMgrCoreQueue<QueueType>::logger_ =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("AppMgrCore"));

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
            LOG4CXX_INFO_EXT(logger_, " AppMgrCoreQueues constructed!");
        }
        else
        {
            LOG4CXX_ERROR_EXT(logger_, " AppMgrCoreQueues not constructed! Check if params are non-null!");
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
            LOG4CXX_INFO_EXT(logger_, "Emptying queue elements");
            mMtx.Lock();
            while( mQueue.size() > 0 )
            {
                mQueue.pop();
            }
            mMtx.Unlock();
            LOG4CXX_INFO_EXT(logger_, "Queue empty.");
        }

        LOG4CXX_INFO_EXT(logger_, " AppMgrCoreQueues detructed!");
    }

    /**
     * \brief execute inner threads
     */
    template< class QueueType >
    void AppMgrCoreQueue<QueueType>::executeThreads()
    {
        LOG4CXX_INFO_EXT(logger_, " Threads are being started!");
        mThread->Start(false);

        LOG4CXX_INFO_EXT(logger_, " Threads have been started!");
    }

    /**
     * \brief push a message to a queue
     * \param message a message being pushed
     */
    template< class QueueType >
    void AppMgrCoreQueue<QueueType>::pushMessage( QueueType message )
    {
        LOG4CXX_INFO_EXT(logger_, " Pushing a message...");
        mMtx.Lock();

        mQueue.push(message);

        mMtx.Unlock();

        mBinarySemaphore.Notify();
        LOG4CXX_INFO_EXT(logger_, " Pushed a message");
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
            LOG4CXX_ERROR_EXT(logger_, " pThis should be non-null!");
            return 0;
        }
        if(!mCallbackFn)
        {
            LOG4CXX_ERROR_EXT(logger_, " callback function pointer should be non-null!");
            return 0;
        }
        while(true)
        {
            bool result = false;
            while ( mMtx.Lock(), result = !mQueue.empty(), mMtx.Unlock(), result )
            {
                LOG4CXX_INFO_EXT(logger_, "Handling message in queue.");
                mMtx.Lock();
                QueueType msg = mQueue.front();
                mQueue.pop();
                mMtx.Unlock();

                mCallbackFn( msg, pThis );
                LOG4CXX_INFO_EXT(logger_, "Message handled.");
            }
            mBinarySemaphore.Wait();
        }
    }

    template class AppMgrCoreQueue<NsRPC2Communication::RPC2Command*>;
    template class AppMgrCoreQueue<Message>;
    template class AppMgrCoreQueue<NsSmartDeviceLinkRPC::SDLRPCMessage*>;

}
