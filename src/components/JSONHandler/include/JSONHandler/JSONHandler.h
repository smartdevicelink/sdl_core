#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS 

#include "Logger.hpp"
#include "JSONHandler/MessageQueue.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "IProtocolObserver.hpp"
#include "ProtocolHandler.hpp"

/**
 * \class JSONHandler
 * \brief Class for handling message exchange between protocol layer and Application Manager.
 * Receives AppLink Json message from Protocol layer, creates corresponding object and sends it to Application Manager.
 * Receives AppLink message object from Application manager, serializes it into Json string and sends to Protocol Layer.
*/
class JSONHandler : public AxisCore::IProtocolObserver
{
public:
    /**
     * \brief Constructor
     * \param protocolHandler Pointer to Protocol Layer handler for message exchange.
    */
    JSONHandler( AxisCore::ProtocolHandler * protocolHandler = 0 );

    /**
     * \brief Destructor
    */
    virtual ~JSONHandler();

    /*Methods from IProtocolObserver*/
    /**
     * \brief Sets pointer for Protocol layer handler for message exchange.
     * \param protocolHandler Pointer to Protocol layer handler.
     */
    void setProtocolHandler( AxisCore::ProtocolHandler * protocolHandler );

    /**
     * \brief Callback for Protocol layer handler to notify of session start.
     * \param sessionID ID of started session
     * \param hashCode Hash Code of the started session to use in finishing the session.
     */
    void sessionStartedCallback(const UInt8 sessionID, const UInt32 hashCode);

    /**
     * \brief Callback for Protocol layer handler to notify of session end.
     * \param sessionID ID of ended session.
     */
    void sessionEndedCallback(const UInt8 sessionID);

    /**
     * \brief Callback for Protocol layer handler to notify of message received.
     * \param #{3:name} ID of session in which message was received.
     * \param messageID ID of received message.
     * \param dataSize Size of received message in bytes.
     */
    void dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize);
    /*end of methods from IProtocolObserver*/

    /*Methods for IRPCMessagesObserver*/
    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
     * \param #{3:name} Pointer to object of the class implementing IRPCMessagesObserver
     * \sa IRPCMessagesObserver
     */
    void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver );

    /**
     * \brief Method for sending message to Mobile Application. 
     * \param message Pointer to base class of AppLink Json object 
     * to be serialized to Json message and sent to mobile App.
     * \param sessionId ID of the session the message was received within.
     */
    void sendRPCMessage( const AppLinkRPC::ALRPCMessage * message, unsigned char sessionId );   
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
    AxisCore::ProtocolHandler *         mProtocolHandler;

    /**
      *\brief Current session ID
      * TODO: to be removed: sessionID's are going to be chained with messages.
    */
    UInt8                                     mSessionID;
    /* End IProtocolObserver data */

    /**
      *\brief Queue of messages from Mobile Application.
      *\sa MessageQueue
    */
    MessageQueue<std::string>          mIncomingMessages;

    /**
      *\brief Thread for handling messages from Mobile Application.
    */
    pthread_t             mWaitForIncomingMessagesThread;

    /**
      *\brief Queue of messages to Mobile Application.
      *\sa MessageQueue
    */
    MessageQueue<const AppLinkRPC::ALRPCMessage*>    mOutgoingMessages;

    /**
      *\brief Thread for handling messages to Mobile Application.
    */
    pthread_t             mWaitForOutgoingMessagesThread;

};

#endif