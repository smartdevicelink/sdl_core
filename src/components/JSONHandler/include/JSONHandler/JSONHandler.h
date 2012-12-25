/**
* \file JSONHandler.hpp
* \brief JSONHandler class header.
* \author PVyshnevska
*/


#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS 

#include "Logger.hpp"
#include "JSONHandler/MessageQueue.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "JSONHandler/IJsonHandler.h"
#include "ProtocolHandler/AppLinkRawMessage.h"
#include "ProtocolHandler/IProtocolObserver.h"
#include "ProtocolHandler/ProtocolHandler.h"

const unsigned char RPC_REQUEST = 0x0;
const unsigned char RPC_RESPONSE = 0x1;
const unsigned char RPC_NOTIFICATION = 0x2;
const unsigned char RPC_UNKNOWN = 0xF;

/**
 * \class JSONHandler
 * \brief Class for handling message exchange between protocol layer and Application Manager.
 * Receives AppLink Json message from Protocol layer, creates corresponding object and sends it to Application Manager.
 * Receives AppLink message object from Application manager, serializes it into Json string and sends to Protocol Layer.
*/
class JSONHandler : public IJsonHandler, public NsProtocolHandler::IProtocolObserver
{
public:
    /**
     * \brief Constructor
     * \param protocolHandler Pointer to Protocol Layer handler for message exchange.
    */
    JSONHandler( NsProtocolHandler::ProtocolHandler * protocolHandler = 0 );

    /**
     * \brief Destructor
    */
    virtual ~JSONHandler();

    /*Methods from IProtocolObserver*/
    /**
     * \brief Sets pointer for Protocol layer handler for message exchange.
     * \param protocolHandler Pointer to Protocol layer handler.
     */
    void setProtocolHandler( NsProtocolHandler::ProtocolHandler * protocolHandler );

    /**
     * \brief Callback for Protocol layer handler to notify of message received.
     * \param message Object containing received data, size of it and connection key.
     */
    void onDataReceivedCallback( const NsProtocolHandler::AppLinkRawMessage * message );
    /*end of methods from IProtocolObserver*/

    /*Methods for IRPCMessagesObserver*/
    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
     * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
     * \sa IRPCMessagesObserver
     */
    void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver );

    /**
     * \brief Method for sending message to Mobile Application. 
     * \param message Pointer to base class of AppLink Json object 
     * to be serialized to Json message and sent to mobile App.
     * \param sessionId ID of the session the message was received within.
     */
    void sendRPCMessage( const NsAppLinkRPC::ALRPCMessage * message, int connectionKey );
    /*End of methods for IRPCMessagesObserver*/
       
protected:
    /**
     * \brief Static method for handling messages from Mobile application.
     * \param params Pointer to JSONHandler instance.
     */
    static void * waitForIncomingMessages( void * params );

    /**
     * \brief Static method for handling messages to Mobile application.
     * \param params Pointer to JSONHandler instance.
     */
    static void * waitForOutgoingMessages( void * params );

    /**
     * \brief Helper method for clearing Json message from empty spaces
     * in order for it to be parsed correctly by Json library.
     * \param input Json string.
     * \return Json string cleared from empty spaces.
     */
    std::string clearEmptySpaces( const std::string & input );

    NsAppLinkRPC::ALRPCMessage * handleIncomingMessageProtocolV1(
            const NsProtocolHandler::AppLinkRawMessage * message );

    NsAppLinkRPC::ALRPCMessage * handleIncomingMessageProtocolV2(
            const NsProtocolHandler::AppLinkRawMessage * message );

    NsProtocolHandler::AppLinkRawMessage * handleOutgoingMessageProtocolV1( 
            int connectionKey, const NsAppLinkRPC::ALRPCMessage *  message );

    NsProtocolHandler::AppLinkRawMessage * handleOutgoingMessageProtocolV2( 
            int connectionKey, const NsAppLinkRPC::ALRPCMessage *  message );
    
private:
    /**
      *\brief For logging.
    */
    static log4cplus::Logger                     mLogger;

    /**
      *\brief Points on instance of class implementing RPC handling (Application Manager).
    */
    IRPCMessagesObserver *             mMessagesObserver;

    /* Data for IProtocolObserver */
    /**
      *\brief Points on instance of Protocol layer handler for message exchange.
    */
    NsProtocolHandler::ProtocolHandler *         mProtocolHandler;

    /* End IProtocolObserver data */

    /**
      *\brief Queue of messages from Mobile Application.
      *\sa MessageQueue
    */
    MessageQueue<const NsProtocolHandler::AppLinkRawMessage*>          mIncomingMessages;

    /**
      *\brief Thread for handling messages from Mobile Application.
    */
    pthread_t             mWaitForIncomingMessagesThread;

    /**
      *\brief Queue of messages to Mobile Application.
      *\sa MessageQueue
    */
    MessageQueue<std::pair<int,const NsAppLinkRPC::ALRPCMessage*>>    mOutgoingMessages;

    /**
      *\brief Thread for handling messages to Mobile Application.
    */
    pthread_t             mWaitForOutgoingMessagesThread;

};

#endif  //  JSONHANDLER_CLASS