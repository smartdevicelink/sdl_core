/**
* \file SmartDeviceLinkRawMessage.h
* \brief SmartDeviceLinkRawMessage class header file.
* 
* Copyright (c) 2013 Ford Motor Company
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