
class MobileRPCMessage;

class MobileRPCResponse : public MobileRPCMessage
{
public:
    MobileRPCResponse( unsigned int protocolVersion, unsigned int correlationID = 0 );
    ~MobileRPCResponse();

    /* data */
};