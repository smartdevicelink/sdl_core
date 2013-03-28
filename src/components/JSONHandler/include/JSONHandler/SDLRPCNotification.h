/**
* \file SDLRPCNotification.hpp
* \brief Class SDLRPCNotification header.
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


#ifndef ALRPCNOTIFICATION_INCLUDE
#define ALRPCNOTIFICATION_INCLUDE

#include "SDLRPCMessage.h"

/**
 * \namespace NsSmartDeviceLinkRPC
 * \brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/ 
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCNotification
     * \brief Base class for SmartDeviceLink Json notification messages.
     */
    class SDLRPCNotification : public SDLRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2)
        */
        SDLRPCNotification( unsigned int protocolVersion );

        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2).
         * \param methodID ID of RPC called by message.
        */
        SDLRPCNotification( unsigned int protocolVersion, int methodID);

        /**
         * \brief Default Constructor
        */
        SDLRPCNotification(void);

        /**
         * \brief Destructor
        */
        virtual ~SDLRPCNotification();

    };

}

#endif //  ALRPCNOTIFICATION_INCLUDE
