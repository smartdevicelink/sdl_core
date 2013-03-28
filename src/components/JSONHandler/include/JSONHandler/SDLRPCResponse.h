/**
* \file SDLRPCResponse.h
* \brief Class SDLRPCResponse header.
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


#ifndef ALRPCRESPONSE_INCLUDE
#define ALRPCRESPONSE_INCLUDE

#include <string>
#include "SDLRPCMessage.h"
#include "JSONHandler/SDLRPCObjects/V2/Result.h"

/**
  *\namespace NsSmartDeviceLinkRPC
  *\brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCResponse
     * \brief Base class for SmartDeviceLink Json response message classes.
    */
    class SDLRPCResponse : public SDLRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol in message.
        */
        SDLRPCResponse(unsigned int protocolVersion);

        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol in message.
         * \param methodID ID of RPC called by message.
        */
        SDLRPCResponse(unsigned int protocolVersion, int methodID);

        /**
         * \brief Default Constructor
        */
        SDLRPCResponse(void);

        /**
         * \brief Destructor
        */
        virtual ~SDLRPCResponse();

        /**
         * \brief Getter for correlation id.
         * \return Correlation ID of Json Message
         */
        virtual unsigned int getCorrelationID() const;

        /**
         * \brief Setter for correlation id.
         * \param correlationID Correlation ID of Json message
         */
        virtual void setCorrelationID(unsigned int correlationID);

        bool get_success(void) const;
        const NsSmartDeviceLinkRPCV2::Result& get_resultCode(void) const;
        const std::string* get_info(void) const;

        bool set_success(bool success_);
        bool set_resultCode(const NsSmartDeviceLinkRPCV2::Result& resultCode_);

        void reset_info(void);
        bool set_info(const std::string& info_);

    protected:
        /**
          *\brief Correlation Id of Json message.
        */
        unsigned int    mCorrelationID;

        /**
        true, if successful
        false, if failed
        */
        bool success;

        ///  See Result
        NsSmartDeviceLinkRPCV2::Result resultCode;

        ///  Provides additional human readable info regarding the result.
        std::string* info;    //!< (1000)
    };
}

#endif  // ALRPCRESPONSE_INCLUDE
