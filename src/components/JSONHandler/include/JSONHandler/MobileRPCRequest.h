#ifndef MOBILERPCREQUEST_CLASS
#define MOBILERPCREQUEST_CLASS value

class MobileRPCMessage;

class MobileRPCRequest : public MobileRPCMessage
{
public:
    MobileRPCRequest( unsigned int protocolVersion );
    MobileRPCRequest( unsigned int protocolVersion, std::string functionName );
    MobileRPCRequest( unsigned int protocolVersion, unsigned int functionID );
    MobileRPCRequest( const MobileRPCMessage& message );
    virtual ~MobileRPCRequest();

    /* data */
};

#endif