/**
* \file SmartDeviceLinkRawMessage.h
* \brief SmartDeviceLinkRawMessage class header file.
* 
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

#ifndef SMARTDEVICELINKRAWMESSAGE_CLASS
#define SMARTDEVICELINKRAWMESSAGE_CLASS

/**
  *\namespace NsProtocolHandler
  *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
*/

namespace NsProtocolHandler
{
    /**
     * \class SmartDeviceLinkRawMessage
     * \brief Class-wraper for information about message for interchanging between components.
    */
    class SmartDeviceLinkRawMessage
    {
    public:
        /**
         * \brief Constructor
         * \param connectionKey Identifier of connection within wich message is transferred
         * \param protocolVersion Version of protocol of the message
         * \param data Message string
         * \param dataSize Message size 
        */
        SmartDeviceLinkRawMessage(int connectionKey,
                            unsigned int protocolVersion,
                            unsigned char * data,
                            unsigned int dataSize);

        /**
         * \brief Destructor
        */
        ~SmartDeviceLinkRawMessage();

        /**
         * \brief Getter for connection identifier
         */
        int getConnectionKey() const;

        /**
         * \brief Getter for message string
         */
        unsigned char * getData() const;

        /**
         * \brief Getter for message size
         */
        unsigned int getDataSize() const;

        /**
         * \brief Getter for protocol version
         */
        unsigned int getProtocolVersion() const;
    
    private:

        /**
         * \brief Connection Identifier
         * Obtained from \saCConnectionHandler
         */
        int mConnectionKey;

        /**
         * \brief Message string
         */
        unsigned char * mData;

        /**
         * \brief Size of message
         */
        unsigned int mDataSize;

        /**
         * \brief Version of SmartDeviceLink protocol (currently 1,2)
         * used for tranferring message.
         */
        unsigned int mProtocolVersion;
    };
}

#endif //  SMARTDEVICELINKRAWMESSAGE_CLASS
