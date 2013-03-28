/**
* \file SDLRPCRequest.hpp
* \brief Class SDLRPCRequest header.
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


#ifndef ALRPCREQUEST_INCLUDE
#define ALRPCREQUEST_INCLUDE

#include "SDLRPCMessage.h"

/**
  *\namespace NsSmartDeviceLinkRPC
  *\brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCRequest
     * \brief Base class for SmartDeviceLink Json request messages.
    */
    class SDLRPCRequest : public SDLRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2)
        */
        SDLRPCRequest( unsigned int protocolVersion );

        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2)
         * \param methodID Id of RPC called by message
        */
        SDLRPCRequest( unsigned int protocolVersion, int methodId);

        /**
         * \brief Default Constructor
        */
        SDLRPCRequest(void);

        /**
         * \brief Destructor
        */
        virtual ~SDLRPCRequest();

        /**
         * \brief Getter for correlation id of JSON message.
         * \return correlation id of JSON message.
         */
        virtual unsigned int getCorrelationID() const;

        /**
         * \brief Setter for correlation id of JSON message.
         * \param correlationID Correlation ID of JSON message.
         * \return type description
         */
        virtual void setCorrelationID( unsigned int correlationID );

    private:
        /**
          *\brief Correlation id of JSON message.
        */
        unsigned int 	mCorrelationID;

    };

}

#endif  // ALRPCREQUEST_INCLUDE
