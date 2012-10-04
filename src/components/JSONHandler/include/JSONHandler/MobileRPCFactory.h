
class MobileRPCResponse;
class MobileRPCRequest;
class MobileRPCNotification;

class MobileRPCFactory
{
public:
    MobileRPCFactory(arguments);
    ~MobileRPCFactory();

    MobileRPCResponse createResponse ( const MobileRPCRequest & request );
    MobileRPCResponse createResponse ( );
    
    MobileRPCNotification createNotification ( const MobileRPCRequest & request );
    MobileRPCNotification createNotification ( );

    /* data */
};