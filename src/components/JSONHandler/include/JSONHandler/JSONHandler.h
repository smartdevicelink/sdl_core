#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS value

#include <json/value.h>

#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"
#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/MobileRPCNotification.h"
#include "JSONHandler/MobileRPCFactory.h"

#include "IProtocolObserver.hpp"
#include "ProtocolHandler.hpp"

class MobileRPCMessage;

class JSONHandler : public IProtocolObserver
{
public:
    JSONHandler( ProtocolHandler * protocolHandler = 0 );
    virtual ~JSONHandler();

    /*Methods from IProtocolObserver*/
    void setProtocolHandler( ProtocolHandler * protocolHandler );
    void sessionStartedCallback(const UInt8 sessionID);
    void sessionEndedCallback(const UInt8 sessionID);
    void dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize);
    /*end of methods from IProtocolObserver*/

    MobileRPCMessage * createObjectFromJSON( const std::string & jsonString );
    std::string serializeObjectToJSON( const MobileRPCMessage & mobileRPCObject );

    static Json::Value getParametersFromJSON( const std::string & jsonString );
    const MobileRPCFactory * getFactory() const;

    MobileRPCMessage * getRPCObject() const;

protected:
    MobileRPCMessage * checkMessageTypeForProtocol1 ( const Json::Value & root );
    MobileRPCRequest * generateRequestVersion1 ( const Json::Value & root );
    MobileRPCResponse * generateResponseVersion1( const Json::Value & root );
    MobileRPCNotification * generateNotificationVersion1( const Json::Value & root );
    MobileRPCMessage * fillMessageWithData ( const Json::Value & jsonMessage, 
                MobileRPCMessage * message );
    std::string serializeObjectToJSONProtocol1 ( const MobileRPCMessage & mobileRPCObject );
    
private:
    /* data */
    MobileRPCFactory * mFactory;
    ProtocolHandler *mProtocolHandler;
    MobileRPCMessage * mCurrentMessage;

};

#endif