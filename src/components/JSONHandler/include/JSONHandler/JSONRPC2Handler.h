/**
* \file JSONRPC2Handler.hpp
* \brief JSONRPC2Handler class header.
* \author PVyshnevska
*/


#ifndef JSONRPC2HANDLER_CLASS
#define JSONRPC2HANDLER_CLASS


#include <json/value.h>
#include "mb_controller.hpp"
#include "Logger.hpp"
#include "JSONHandler/IRPC2CommandsObserver.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/MessageQueue.h"

/**
 * \class JSONRPC2Handler
 * \brief Class for handling message exchange between RPCBus and Application Manager.
 * Receives RPC2 Json message from RPCBus (HMI), creates corresponding object and sends it to Application Manager.
 * Receives RPC2 message object from Application manager, serializes it into Json object and sends to RPCBus (HMI). 
*/
class JSONRPC2Handler : public NsMessageBroker::CMessageBrokerController
{
public:
    /**
     * \brief Constructor
     * \param address Address of Message Broker (RPCBus) server.
     * \param port Port of Message Broker (RPCBus) server.
    */
    JSONRPC2Handler( const std::string& address, uint16_t port );

    /**
     * \brief Destructor
    */
    ~JSONRPC2Handler();

    /*Methods from CMessageBrokerController*/
    /**
     * \brief Called on receiving response message from RPCBus.
     * \param method Name of corresponding request method that was sent previously to RPCBus.
     * \param root Received Json object.
     */
    void processResponse(std::string method, Json::Value& root);

    /**
     * \brief Called on receiving request message from RPCBus.
     * \param root Received Json object.
     */
    void processRequest(Json::Value& root);

    /**
     * \brief Called on receiving notification message from RPCBus.
     * \param root Received Json object.
     */
    void processNotification(Json::Value& root);

    /**
     * \brief Notifies RPCBus of the will to receive specific notifications.
     */
    void subscribeToNotifications( );
    /*End of ethods from CMessageBrokerController*/

    /*Methods for IRPC2CommandsObserver*/
    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (App Manager).
     * \param commandsObserver Pointer to implementation of IRPC2CommandsObserver.
     * \sa IRPC2CommandsObserver.
     */
    void setRPC2CommandsObserver( IRPC2CommandsObserver * commandsObserver );

    /**
     * \brief Sends notification to RPCBus.
     * \param command Json RPC2 notification to be sent to RPCBus.
     */
    void sendNotification( const RPC2Communication::RPC2Notification * command );

    /**
     * \brief Sends response to RPCBus.
     * \param command Json RPC2 response to be sent to RPCBus.
     */
    void sendResponse( const RPC2Communication::RPC2Response * command );

    /**
     * \brief Sends request to RPCBus.
     * \param command Json RPC2 request to be sent to RPCBus.
     */
    void sendRequest( const RPC2Communication::RPC2Request * command );
    /*End of methods for IRPC2CommandsObserver*/

private:
    /**
     * \brief Static method for handling requests & notifications from RPCBus.
     * \param params Pointer to JSONRPC2Handler instance.
     */
    static void * waitForCommandsFromHMI( void * params );

    /**
     * \brief Static method for handling responses from RPCBus.
     * \param params Pointer to JSONRPC2Handler instance.
     */
    static void * waitForResponsesFromHMI( void * params );

    /**
     * \brief Static method for handling responses to RPCBus.
     * \param params Pointer to JSONRPC2Handler instance.
     */
    static void * waitForResponsesToHMI( void * params );

    /**
     * \brief Static method for handling requests to RPCBus.
     * \param params Pointer to JSONRPC2Handler instance.
     */
    static void * waitForRequestsToHMI( void * params );

    /**
     * \brief Static method for handling notifications to RPCBus.
     * \param params Pointer to JSONRPC2Handler instance.
     */
    static void * waitForNotificationsToHMI( void * params );

private:
    /**
      *\brief For logging.
    */
    static log4cplus::Logger                              mLogger;

    /**
     * \struct ResponseContainer
     * \brief Used for associating request method name to corresponding response.
     * TODO - not needed with latest version of code generator?
    */
    struct ResponseContainer
    {
        /**
          *\brief Request method name.
        */
        std::string methodName;

        /**
          *\brief Response.
        */
        Json::Value response;
    };

    /**
      *\brief Points on instance of class implementing RPC handling (Application Manager). 
    */
    IRPC2CommandsObserver *                     mCommandsObserver;

    /**
      *\brief Queue of requests & notifications from RPCBus.
      *\sa MessageQueue
    */
    MessageQueue<Json::Value>                    mCommandsFromHMI;

    /**
      *\brief Thread for handling requests & notifications from HMI.
    */
    pthread_t                             mWaitForCommandsFromHMI;

    /**
      *\brief Thread for handling responses from HMI.
    */
    pthread_t                            mWaitForResponsesFromHMI;

    /**
      *\brief Queue of responses from HMI.
      * \sa MessageQueue
    */
    MessageQueue<ResponseContainer>             mResponsesFromHMI;

    /**
      *\brief Queue of requests to HMI.
      * \sa MessageQueue
    */
    MessageQueue<const RPC2Communication::RPC2Request*> mRequestsToHMI;

    /**
      *\brief Queue of responses to HMI.
      * \sa MessageQueue
    */
    MessageQueue<const RPC2Communication::RPC2Response*> mResponsesToHMI;

    /**
      *\brief Queue of notifications to HMI.
      * \sa MessageQueue
    */
    MessageQueue<const RPC2Communication::RPC2Notification*> mNotificationsToHMI;

    /**
      *\brief Thread for handling requests to HMI.
    */
    pthread_t                               mWaitForRequestsToHMI;

    /**
      *\brief Thread for handling responses to HMI.
    */
    pthread_t                               mWaitForResponsesToHMI;

    /**
      *\brief Thread for handling notifications to HMI.
    */
    pthread_t                               mWaitForNotificationsToHMI;
};

#endif  //  JSONRPC2HANDLER_CLASS
