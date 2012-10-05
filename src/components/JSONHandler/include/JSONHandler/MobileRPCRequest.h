
class MobileRPCMessage;

class MobileRPCRequest : public MobileRPCMessage
{
public:
    MobileRPCRequest( unsigned int protocolVersion, unsigned int correlationID = 0 );
    MobileRPCRequest( unsigned int protocolVersion, unsigned int correlationID, 
        Parameters params, std::string functionName );
    MobileRPCRequest( unsigned int protocolVersion, unsigned int correlationID, 
        Parameters params, unsigned int functionID, unsigned int messageSize );
    ~MobileRPCRequest();

    /* data */
};