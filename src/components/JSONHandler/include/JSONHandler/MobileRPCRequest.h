
class MobileRPCMessage;

class MobileRPCRequest : public MobileRPCMessage
{
public:
    MobileRPCRequest( unsigned int protocolVersion );
    MobileRPCRequest( unsigned int protocolVersion, std::string functionName );
    MobileRPCRequest( unsigned int protocolVersion, unsigned int functionID );
    virtual ~MobileRPCRequest();

    /* data */
};