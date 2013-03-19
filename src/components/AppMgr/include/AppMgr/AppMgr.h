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

#ifndef APPMGR_H
#define APPMGR_H

#include "JSONHandler/IRPCMessagesObserver.h"
#include "JSONHandler/IRPC2CommandsObserver.h"
#include "ConnectionHandler/IConnectionHandlerObserver.hpp"

class JSONHandler;
class SDLRPCMessage;
class JSONRPC2Handler;

namespace log4cplus
{
    class Logger;
}

namespace NsRPC2Communication
{
    class RPC2Command;
}

namespace NsConnectionHandler
{
    class IDevicesDiscoveryStarter;
}

namespace NsAppManager
{
    class AppMgrRegistry;
    class AppMgrCore;
    class AppFactory;

    /**
    * \brief a main app manager class which acts like container for other classes
    */
    class AppMgr: public IRPCMessagesObserver, public IRPC2CommandsObserver, public NsConnectionHandler::IConnectionHandlerObserver
    {
    public:

        /**
         * \brief Returning class instance
         * \return class instance
         */
        static AppMgr& getInstance();

        /**
         * \brief callback to proceed received mobile message
         * \param message the received message
         * \param connectionKey key of a connection associated with application that sent the message
         */
        virtual void onMessageReceivedCallback( NsSmartDeviceLinkRPC::SDLRPCMessage * message, int connectionKey );

        /**
         * \brief callback to proceed received RPC2 command
         * \param command the received command
         */
        virtual void onCommandReceivedCallback( NsRPC2Communication::RPC2Command * command );

        /**
         * \brief Available devices list updated.
         *
         * Called when device scanning initiated with scanForNewDevices
         * is completed.
         *
         * \param deviceList New list of available devices.
         **/
        virtual void onDeviceListUpdated(const NsConnectionHandler::tDeviceList & deviceList);

        /**
         * \brief callback which is called upon session starting
         * \param deviceHandle
         * \param sessionKey
         */
        virtual void onSessionStartedCallback(NsConnectionHandler::tDeviceHandle deviceHandle, int sessionKey, int firstSessionKey);

        /**
         * \brief callback which is called upon session ending
         * \param sessionKey
         */
        virtual void onSessionEndedCallback(int sessionKey, int firstSessionKey);

        /**
         * \brief Sets Json mobile handler instance
         * \param handler Json mobile handler
         */
        void setJsonHandler(JSONHandler* handler);

        /**
         * \brief Sets Json RPC2 handler instance
         * \param handler Json RPC2 handler
         */
        void setJsonRPC2Handler(JSONRPC2Handler* handler);

        /**
         * \brief Sets connection handler instance
         * \param handler connection handler
         */
        void setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter *handler);

        /**
         * \brief method to execute threads.
         */
        void executeThreads();

    private:

        /**
         * \brief Default class destructor
         */
        virtual ~AppMgr();

        /**
         * \brief Copy constructor
         */
        AppMgr(const AppMgr&);

        /**
         * \brief Default class constructor
         */
        AppMgr();

        static log4cplus::Logger mLogger;
    };

} // namespace NsAppManager

#endif // APPMGR_H
