#include <json/value.h>
#include <json/reader.h>
#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"
#include "JSONHandler/MobileRPCResponse.h"

class MobileRPCMessage;

class JSONHandler
{
public:
    JSONHandler();
    ~JSONHandler();

    static MobileRPCMessage * createObjectFromJSON( const std::string & jsonString );
    static std::string serializeObjectToJSON( const MobileRPCMessage & mobileRPCObject );

protected:
    static MobileRPCMessage * checkMessageTypeForProtocol1 ( const Json::Value & root );
    static MobileRPCRequest * generateRequestVersion1 ( const Json::Value & root );
    static MobileRPCMessage * fillMessageWithData ( const Json::Value & jsonMessage, 
                MobileRPCMessage * message );
    /* data */

};