#ifndef MOBILERPCRESPONSE_CLASS
#define MOBILERPCRESPONSE_CLASS value

class MobileRPCMessage;

class MobileRPCResponse : public MobileRPCMessage
{
public:
    MobileRPCResponse( unsigned int protocolVersion );
    MobileRPCResponse( unsigned int protocolVersion, std::string functionName );
    MobileRPCResponse( unsigned int protocolVersion, unsigned int functionID );
    virtual ~MobileRPCResponse();

    /* data */
};

#endif