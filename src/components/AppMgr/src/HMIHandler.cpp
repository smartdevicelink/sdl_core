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

#include "AppMgr/HMIHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"

namespace NsAppManager
{

    log4cxx::LoggerPtr HMIHandler::logger_ =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("AppMgrCore"));

    /**
     * \brief Returning class instance
     * \return class instance
     */
    HMIHandler& HMIHandler::getInstance()
    {
        static HMIHandler instance;
        return instance;
    }

    /**
     * \brief set handler ready state
     * \param ready ready state
     */
    void HMIHandler::setReadyState(bool ready)
    {
        if(mJSONRPC2Handler)
        {
            m_bHMIReady = ready;
        }
        else
        {
            LOG4CXX_ERROR_EXT(logger_, " HMIHandler is about to set ready, but is null!");
        }
    }

    /**
     * \brief send notification via associated handler
     * \param command notification to send
     */
    void HMIHandler::sendNotification(const NsRPC2Communication::RPC2Notification *command)
    {
        if(m_bHMIReady)
        {
            if(mJSONRPC2Handler)
            {
                LOG4CXX_INFO_EXT(logger_, " Sending a notification " << command->getMethod());
                mJSONRPC2Handler->sendNotification(command);
            }
        }
        else
        {
            LOG4CXX_ERROR_EXT(logger_, " HMIHandler not ready yet!");
        }
    }

    /**
     * \brief send response via associated handler
     * \param command response to send
     */
    void HMIHandler::sendResponse(const NsRPC2Communication::RPC2Response *command)
    {
        if(m_bHMIReady)
        {
            if(mJSONRPC2Handler)
            {
                LOG4CXX_INFO_EXT(logger_, " Sending a response " << command->getMethod());
                mJSONRPC2Handler->sendResponse(command);
            }
        }
        else
        {
            LOG4CXX_ERROR_EXT(logger_, " HMIHandler not ready yet!");
        }
    }

    /**
     * \brief send request via associated handler
     * \param command request to send
     */
    void HMIHandler::sendRequest(const NsRPC2Communication::RPC2Request *command)
    {
        if(m_bHMIReady)
        {
            if(mJSONRPC2Handler)
            {
                LOG4CXX_INFO_EXT(logger_, " Sending a request " << command->getMethod());
                mJSONRPC2Handler->sendRequest(command);
            }
        }
        else
        {
            LOG4CXX_ERROR_EXT(logger_, " HMIHandler not ready yet!");
        }
    }

    /**
     * \brief set Json RPC2 handler
     * \param handler Json RPC2 handler instance
     */
    void HMIHandler::setJsonRPC2Handler(JSONRPC2Handler *handler)
    {
        if(!handler)
        {
            LOG4CXX_ERROR_EXT(logger_, "A null pointer is being assigned - is this the intent?");
            return;
        }
        mJSONRPC2Handler = handler;
    }

    /**
     * \brief get Json RPC2 handler
     * \return Json RPC2 handler instance
     */
    JSONRPC2Handler *HMIHandler::getJsonRPC2Handler() const
    {
        if(!mJSONRPC2Handler)
        {
            LOG4CXX_ERROR_EXT(logger_, "JSON RPC2 handler hasn't yet been assigned, but an attempt to retrieve it has been made! Face a core dump soon...(((");
        }
        return mJSONRPC2Handler;
    }

    /**
     * \brief Default class constructor
     */
    HMIHandler::HMIHandler()
        :mJSONRPC2Handler(0)
        ,m_bHMIReady(false)
    {
        LOG4CXX_INFO_EXT(logger_, " HMIHandler constructed!");
    }

    /**
     * \brief Copy constructor
     */
    HMIHandler::HMIHandler(const HMIHandler &)
        :mJSONRPC2Handler(0)
        ,m_bHMIReady(false)
    {
    }

}
