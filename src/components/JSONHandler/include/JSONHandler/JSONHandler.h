#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS value

#include <json/value.h>

#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"
#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/MobileRPCNotification.h"
#include "JSONHandler/MobileRPCFactory.h"
#include "JSONHandler/MessageQueue.h"

#include "JSONHandler/IRPCMessagesObserver.h"

#include "IProtocolObserver.hpp"
#include "ProtocolHandler.hpp"

class MobileRPCMessage;

class JSONHandler : public AxisCore::IProtocolObserver
{
    friend class TestJSONHandler;
public:
    JSONHandler( AxisCore::ProtocolHandler * protocolHandler = 0 );
    virtual ~JSONHandler();

    /*Methods from IProtocolObserver*/
    void setProtocolHandler( AxisCore::ProtocolHandler * protocolHandler );
    void sessionStartedCallback(const UInt8 sessionID, const UInt32 hashCode);
    void sessionEndedCallback(const UInt8 sessionID);
    void dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize);
    /*end of methods from IProtocolObserver*/

    /*Methods for IRPCMessagesObserver*/
    void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver );
    void sendRPCMessage( const MobileRPCMessage * message );   
    /*End of methods for IRPCMessagesObserver*/
    
    const MobileRPCFactory * getFactory() const;    
    static Json::Value getParametersFromJSON( const std::string & jsonString );

protected:
    MobileRPCMessage * checkMessageTypeForProtocol1 ( const Json::Value & root );
    MobileRPCRequest * generateRequestVersion1 ( const Json::Value & root );
    MobileRPCResponse * generateResponseVersion1( const Json::Value & root );
    MobileRPCNotification * generateNotificationVersion1( const Json::Value & root );
    MobileRPCMessage * fillMessageWithData ( const Json::Value & jsonMessage, 
                MobileRPCMessage * message );
    std::string serializeObjectToJSONProtocol1 ( const MobileRPCMessage & mobileRPCObject );
    static void * waitForIncomingMessages( void * params );
    static void * waitForOutgoingMessages( void * params );
    std::string clearEmptySpaces( const std::string & input );
    MobileRPCMessage * createObjectFromJSON( const std::string & jsonString );
    //////////////////////////////////////////
    std::string serializeObjectToJSON( const MobileRPCMessage & mobileRPCObject );
    Json::Value createJSONFromObject ( const MobileRPCMessage & mobileRPCObject );
    std::string jsonToString( const Json::Value & jsonObject );
    ///////////////////////////////////////////
    
private:
    MobileRPCFactory *                          mFactory;
    IRPCMessagesObserver *             mMessagesObserver;

    /* Data for IProtocolObserver */
    AxisCore::ProtocolHandler *         mProtocolHandler;
    UInt8                                     mSessionID;
    /* End IProtocolObserver data */

    MessageQueue<std::string>          mIncomingMessages;
    pthread_t             mWaitForIncomingMessagesThread;
    MessageQueue<const MobileRPCMessage*>    mOutgoingMessages;
    pthread_t             mWaitForOutgoingMessagesThread;

};

#endif