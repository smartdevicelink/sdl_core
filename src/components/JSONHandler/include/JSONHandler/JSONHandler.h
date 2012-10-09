#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS value

#include <json/value.h>
#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"
#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/MobileRPCNotification.h"
#include "JSONHandler/MobileRPCFactory.h"

class MobileRPCMessage;

class JSONHandler
{
public:
    JSONHandler();
    ~JSONHandler();

    static MobileRPCMessage * createObjectFromJSON( const std::string & jsonString );
    static std::string serializeObjectToJSON( const MobileRPCMessage & mobileRPCObject );

    static Json::Value getParametersFromJSON( const std::string & jsonString );
    static MobileRPCFactory * getFactory();

protected:
    static MobileRPCMessage * checkMessageTypeForProtocol1 ( const Json::Value & root );
    static MobileRPCRequest * generateRequestVersion1 ( const Json::Value & root );
    static MobileRPCResponse * generateResponseVersion1( const Json::Value & root );
    static MobileRPCNotification * generateNotificationVersion1( const Json::Value & root );
    static MobileRPCMessage * fillMessageWithData ( const Json::Value & jsonMessage, 
                MobileRPCMessage * message );
    static std::string serializeObjectToJSONProtocol1 ( const MobileRPCMessage & mobileRPCObject );
    /* data */

private:
    static MobileRPCFactory * mFactory;

};

#endif