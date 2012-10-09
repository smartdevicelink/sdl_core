#ifndef MOBILERPCNOTIFICATION_CLASS
#define MOBILERPCNOTIFICATION_CLASS value

class MobileRPCMessage;

class MobileRPCNotification : public MobileRPCMessage
{
public:
    MobileRPCNotification( unsigned int protocolVersion );
    MobileRPCNotification( unsigned int protocolVersion, std::string functionName );
    MobileRPCNotification( unsigned int protocolVersion, unsigned int functionID );
    virtual ~MobileRPCNotification();

    /* data */
};

#endif