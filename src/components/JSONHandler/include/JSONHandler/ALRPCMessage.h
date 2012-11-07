#ifndef ALRPCMESSAGE_INCLUDE
#define ALRPCMESSAGE_INCLUDE

namespace AppLinkRPC
{
    class ALRPCMessage
    {
    public:
        enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };

    public:
        ALRPCMessage( unsigned int protocolVersion, MessageType messageType );
        ALRPCMessage( unsigned int protocolVersion, MessageType messageType,int methodID);
        virtual ~ALRPCMessage();

        virtual unsigned int getProtocolVersion() const;
        virtual MessageType  getMessageType() const;
        virtual int getMethodId() const;

        virtual void setProtocolVersion( unsigned int protocolVersion );
        virtual void setMessageType( MessageType messageType );
        virtual void setMethodId(int packetID );

    private:
        /* data */
        unsigned int mProtocolVersion;
        MessageType mMessageType;
        int mMethodId;   // function name + "_" + MessageType+ "Id"
    };

}

#endif    //  ALRPCMESSAGE_INCLUDE
