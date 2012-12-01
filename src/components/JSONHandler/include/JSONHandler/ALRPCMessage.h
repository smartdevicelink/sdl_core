/**
* \file ALRPCMessage.hpp
* \brief ALRPCMessage class header.
* \author PVyshnevska
*/


#ifndef ALRPCMESSAGE_INCLUDE
#define ALRPCMESSAGE_INCLUDE

#include <vector>

/**
  *\namespace NsAppLinkRPC
  *\brief Namespace for AppLink JSON protocol related functionality.
*/
namespace NsAppLinkRPC
{
    /**
     * \class ALRPCMessage
     * \brief Base class for AppLink Json messages classes.
    */
    class ALRPCMessage
    {
    public:
        /**
         *\enum MessageType
         *\brief AppLink message types.
        */
        enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };

    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol (currently 1,2)
         * \param messageType Type of AppLink message
        */
        ALRPCMessage( unsigned int protocolVersion, MessageType messageType );

        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol (currently 1,2).
         * \param messageType Type of AppLink message.
         * \param methodID ID of RPC in message.
        */
        ALRPCMessage( unsigned int protocolVersion, MessageType messageType,int methodID);
        /**
         * \brief Destructor
        */
        virtual ~ALRPCMessage();

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
        virtual void setProtocolVersion( unsigned int protocolVersion );

        /**
         * \brief setter for type of message.
        */
        virtual void setMessageType( MessageType messageType );

        /**
         * \brief setter for id of RPC.
        */
        virtual void setMethodId( int packetID );

        /**
         * \brief setter of binary data
         * \param binaryData Vector of binary.
         */
        virtual void setBinaryData( const std::vector<unsigned char> & binaryData );

        /**
         * \brief Getter of binary data
         * \return Pointer to binary data or NULL if doesn't exist.
         */
        virtual const std::vector<unsigned char> * getBinaryData() const;

    private:
        /**
         * \brief Applink protocol version (currently 1,2)
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
        std::vector<unsigned char> * mBinaryData;
    };

}

#endif    //  ALRPCMESSAGE_INCLUDE
