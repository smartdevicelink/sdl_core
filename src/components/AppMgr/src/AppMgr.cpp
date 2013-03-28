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

#include "AppMgr/AppMgr.h"
#include "AppMgr/AppMgrCore.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger AppMgr::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgr"));

    /**
    * \brief Returning class instance
    * \return class instance
    */
    AppMgr& AppMgr::getInstance( )
    {
        static AppMgr appMgr;
        return appMgr;
    }

    /**
    * \brief Default class constructor
    */
    AppMgr::AppMgr()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgr constructed!");
    }

    /**
    * \brief Default class destructor
    */
    AppMgr::~AppMgr()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgr destructed!");
    }

    /**
    * \brief Copy constructor
    */
    AppMgr::AppMgr(const AppMgr &)
    {
    }

    /**
    * \brief Sets Json mobile handler instance
    * \param handler Json mobile handler
    */
    void AppMgr::setJsonHandler(JSONHandler* handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
            return;
        }
        AppMgrCore::getInstance().setJsonHandler( handler );
    }

    /**
    * \brief Sets Json RPC2 handler instance
    * \param handler Json RPC2 handler
    */
    void AppMgr::setJsonRPC2Handler(JSONRPC2Handler *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
            return;
        }
        AppMgrCore::getInstance().setJsonRPC2Handler( handler );
    }

    /**
     * \brief Sets connection handler instance
     * \param handler connection handler
     */
    void AppMgr::setConnectionHandler(connection_handler::IDevicesDiscoveryStarter *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
            return;
        }
        AppMgrCore::getInstance().setConnectionHandler( handler );
    }

    /**
    * \brief callback to proceed received mobile message
    * \param message the received message
    * \param connectionKey key of a connection associated with application that sent the message
    */
    void AppMgr::onMessageReceivedCallback(NsSmartDeviceLinkRPC::SDLRPCMessage * message, int connectionKey)
    {
        if(!message)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command! connection key" << connectionKey);
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Message " << message->getMethodId() << " received from mobile side");
        AppMgrCore::getInstance().pushMobileRPCMessage( message, connectionKey );
    }

    /**
    * \brief callback to proceed received RPC2 command
    * \param command the received command
    */
    void AppMgr::onCommandReceivedCallback(NsRPC2Communication::RPC2Command *command)
    {
        if(!command)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command!");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Message " << command->getMethod() << " received from HMI side");
        AppMgrCore::getInstance().pushRPC2CommunicationMessage(command);
    }

    /**
     * \brief Available devices list updated.
     *
     * Called when device scanning initiated with scanForNewDevices
     * is completed.
     *
     * \param deviceList New list of available devices.
     **/
    void AppMgr::OnDeviceListUpdated(const connection_handler::DeviceList &deviceList)
    {
        AppMgrCore::getInstance().setDeviceList(deviceList);
    }

    void AppMgr::UpdateDeviceList(const connection_handler::DeviceList & DeviceList)
    {
    }

    void AppMgr::RemoveDevice(const connection_handler::DeviceHandle DeviceHandle)
    {
    }

    /**
     * \brief callback which is called upon session starting
     * \param deviceHandle
     * \param sessionKey
     */
    void AppMgr::OnSessionStartedCallback(connection_handler::DeviceHandle deviceHandle, int sessionKey, int firstSessionKey)
    {
        AppMgrCore::getInstance().addDevice(deviceHandle, sessionKey, firstSessionKey);
    }

    /**
     * \brief callback which is called upon session ending
     * \param sessionKey
     */
    void AppMgr::OnSessionEndedCallback(int sessionKey, int firstSessionKey)
    {
        AppMgrCore::getInstance().removeDevice(sessionKey, firstSessionKey);
    }

    /**
    * \brief method to execute threads.
    */
    void AppMgr::executeThreads()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");

        AppMgrCore::getInstance().executeThreads();

        LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
    }

}
