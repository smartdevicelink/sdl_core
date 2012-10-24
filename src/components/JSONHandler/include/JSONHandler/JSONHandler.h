#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS 

#include "JSONHandler/MessageQueue.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "IProtocolObserver.hpp"
#include "ProtocolHandler.hpp"

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
    void sendRPCMessage( const ALRPCMessage * message );   
    /*End of methods for IRPCMessagesObserver*/
       
    //static Json::Value getParametersFromJSON( const std::string & jsonString );

protected:
    static void * waitForIncomingMessages( void * params );
    static void * waitForOutgoingMessages( void * params );
    std::string clearEmptySpaces( const std::string & input );
    
private:
    IRPCMessagesObserver *             mMessagesObserver;

    /* Data for IProtocolObserver */
    AxisCore::ProtocolHandler *         mProtocolHandler;
    UInt8                                     mSessionID;
    /* End IProtocolObserver data */

    MessageQueue<std::string>          mIncomingMessages;
    pthread_t             mWaitForIncomingMessagesThread;
    MessageQueue<const ALRPCMessage*>    mOutgoingMessages;
    pthread_t             mWaitForOutgoingMessagesThread;

};

#endif