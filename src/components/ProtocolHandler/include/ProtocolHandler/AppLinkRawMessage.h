/**
* \file AppLinkRawMessage.h
* \brief AppLinkRawMessage class header file.
* \author PVyshnevska
*/

#ifndef APPLINKRAWMESSAGE_CLASS
#define APPLINKRAWMESSAGE_CLASS

/**
  *\namespace NsProtocolHandler
  *\brief Namespace for AppLink ProtocolHandler related functionality.
*/

namespace NsProtocolHandler
{
    /**
     * \class AppLinkRawMessage
     * \brief Class-wraper for information about message for interchanging between components.
    */
    class AppLinkRawMessage
    {
    public:
        /**
         * \brief Constructor
         * \param connectionKey Identifier of connection within wich message is transferred
         * \param protocolVersion Version of protocol of the message
         * \param data Message string
         * \param dataSize Message size 
        */
        AppLinkRawMessage(int connectionKey,
                            unsigned int protocolVersion,
                            unsigned char * data,
                            unsigned int dataSize);

        /**
         * \brief Destructor
        */
        ~AppLinkRawMessage();

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
         * \brief Version of Applink protocol (currently 1,2)
         * used for tranferring message.
         */
        unsigned int mProtocolVersion;
    };
}

#endif //  APPLINKRAWMESSAGE_CLASS