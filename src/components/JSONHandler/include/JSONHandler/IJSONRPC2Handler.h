/**
* \file IJSONRPC2Handler.h
* \brief IJSONRPC2Handler class header.
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef IJSONRPC2HANDLER_INCLUDE
#define IJSONRPC2HANDLER_INCLUDE

#include "JSONHandler/IRPC2CommandsObserver.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Request.h"
/**
 * \class IJSONRPC2Handler
 * \brief Interface for making a bridge between Application Manager
 * and RPCBus (HMI).
*/
class IJSONRPC2Handler
{
public:
    /**
     * \brief Destructor
    */
    virtual ~IJSONRPC2Handler() {}

    /**
     * \brief Notifies RPCBus of the will to receive specific notifications.
     */
    virtual void subscribeToNotifications( ) = 0;

    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (App Manager).
     * \param commandsObserver Pointer to implementation of IRPC2CommandsObserver.
     * \sa IRPC2CommandsObserver.
     */
    virtual void setRPC2CommandsObserver( IRPC2CommandsObserver * commandsObserver ) = 0;

    /**
     * \brief Sends notification to RPCBus.
     * \param command Json RPC2 notification to be sent to RPCBus.
     */
    virtual void sendNotification( const NsRPC2Communication::RPC2Notification * command ) = 0;

    /**
     * \brief Sends response to RPCBus.
     * \param command Json RPC2 response to be sent to RPCBus.
     */
    virtual void sendResponse( const NsRPC2Communication::RPC2Response * command ) = 0;

    /**
     * \brief Sends request to RPCBus.
     * \param command Json RPC2 request to be sent to RPCBus.
     */
    virtual void sendRequest( const NsRPC2Communication::RPC2Request * command ) = 0;
};

#endif // IJSONRPC2HANDLER_INCLUDE
