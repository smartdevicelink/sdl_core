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
