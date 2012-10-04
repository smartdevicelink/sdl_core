
class MobileRPCMessage;

class JSONHandler
{
public:
    JSONHandler();
    ~JSONHandler();

    static MobileRPCMessage createObjectFromJSON( const std::string & jsonString );
    static std::string serializeObjectToJSON( const MobileRPCMessage & mobileRPCObject );

    /* data */

};