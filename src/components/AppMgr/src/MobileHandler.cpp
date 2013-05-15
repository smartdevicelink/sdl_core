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

#include "AppMgr/MobileHandler.h"
#include "JSONHandler/JSONHandler.h"

namespace NsAppManager
{

    log4cxx::LoggerPtr MobileHandler::logger_ =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("MobileHandler"));

    /**
     * \brief Returning class instance
     * \return class instance
     */
    MobileHandler &MobileHandler::getInstance()
    {
        static MobileHandler instance;
        return instance;
    }

    /**
     * \brief send a message via associated handler
     * \param message message to send
     * \param connectionID id of a connection associated with application that sent the message
     * \param sessionId id of a session associated with application that sent the message
     */
    void MobileHandler::sendRPCMessage(const NsSmartDeviceLinkRPC::SDLRPCMessage *message, int appId)
    {
        if(mJSONHandler)
        {
            LOG4CXX_INFO_EXT(logger_, " Sending a message " << message->getMethodId()
                << " of the type " << message->getMessageType()
                << " with the application id " << appId
                << " to a mobile side");
            mJSONHandler->sendRPCMessage(message, appId);
        }
        else
        {
            LOG4CXX_ERROR_EXT(logger_, " MobileHandler is null!");
        }
    }

    /**
     * \brief set mobile Json handler
     * \param handler mobile Json handler instance
     */
    void MobileHandler::setJsonHandler(JSONHandler *handler)
    {
        if(!handler)
        {
            LOG4CXX_ERROR_EXT(logger_, "A null pointer is being assigned - is this the intent?");
            return;
        }
        mJSONHandler = handler;
    }

    /**
     * \brief get mobile Json handler
     * \return mobile Json handler instance
     */
    JSONHandler *MobileHandler::getJsonHandler() const
    {
        if(!mJSONHandler)
        {
            LOG4CXX_ERROR_EXT(logger_, "JSON Mobile handler hasn't yet been assigned, but an attempt to retrieve it has been made! Face a core dump soon...(((");
        }
        return mJSONHandler;
    }

    /**
     * \brief Default class constructor
     */
    MobileHandler::MobileHandler()
        :mJSONHandler(0)
    {
    }

    /**
     * \brief Copy constructor
     */
    MobileHandler::MobileHandler(const MobileHandler &)
        :mJSONHandler(0)
    {
    }

}
