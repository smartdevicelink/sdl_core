/**
* \file SDLRPCMessage.hpp
* \brief SDLRPCMessage class header.
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


#ifndef ALRPCMESSAGE_INCLUDE
#define ALRPCMESSAGE_INCLUDE

#include <vector>

/**
  *\namespace NsSmartDeviceLinkRPC
  *\brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCMessage
     * \brief Base class for SmartDeviceLink Json messages classes.
    */
    class SDLRPCMessage
    {
    public:
        /**
         *\enum MessageType
         *\brief SmartDeviceLink message types.
        */
        enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };

    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of SmartDeviceLink protocol (currently 1,2)
         * \param messageType Type of SmartDeviceLink message
        */
        SDLRPCMessage(unsigned int protocolVersion, MessageType messageType = UNDEFINED);

        /**
         * \brief Constructor
         * \param protocolVersion Version of SmartDeviceLink protocol (currently 1,2).
         * \param messageType Type of SmartDeviceLink message.
         * \param methodID ID of RPC in message.
        */
        SDLRPCMessage(unsigned int protocolVersion, MessageType messageType, int methodID);
        /**
         * \brief Destructor
        */
        virtual ~SDLRPCMessage();

        /**
         * \brief Getter for protocol version.
        */
        virtual unsigned int getProtocolVersion() const;

        /**
         * \brief Getter for type of message.
        */
        virtual MessageType  getMessageType() const;

        /**
         * \brief Getter for id of RPC.
        */
        virtual int getMethodId() const;

        /**
         * \breif Setter for protocol version.
        */
        virtual void setProtocolVersion(unsigned int protocolVersion);

        /**
         * \brief setter for type of message.
        */
        virtual void setMessageType(MessageType messageType);

        /**
         * \brief setter for id of RPC.
        */
        virtual void setMethodId(int packetID);

        /**
         * \brief setter of binary data
         * \param binaryData Vector of binary.
         */
        virtual void setBinaryData(const std::vector<unsigned char>& binaryData);

        /**
         * \brief Getter of binary data
         * \return Pointer to binary data or NULL if doesn't exist.
         */
        virtual const std::vector<unsigned char>* getBinaryData() const;

        /**
         * \brief Getter for correlation id of JSON message.
         * TODO (PV) it is not supposed to be in base class.
         * \return correlation id of JSON message.
         */
        virtual unsigned int getCorrelationID() const;

        /**
         * \brief Setter for correlation id of JSON message.
         * TODO (PV) it is not supposed to be in base class.
         * \param correlationID Correlation ID of JSON message.
         * \return type description
         */
        virtual void setCorrelationID(unsigned int correlationID);
    private:
        /**
         * \brief SmartDeviceLink protocol version (currently 1,2)
        */
        unsigned int mProtocolVersion;

        /**
         * \brief Type of Message
         * \sa enum MessageType
        */
        MessageType mMessageType;

        /**
         * \brief Id of RPC called in message
         * function name + "_" + MessageType+ "Id"
        */
        int mMethodId;

        /**
          *\brief Array of binary data if it exists
        */
        std::vector<unsigned char>* mBinaryData;

        /**
          *\brief Correlation id of JSON message
          * TODO not supposed to be in base class.
        */
        unsigned int    mCorrelationID;
    };

}

#endif    //  ALRPCMESSAGE_INCLUDE
