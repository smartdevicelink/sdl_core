#ifndef JSONRPC2HANDLER_CLASS
#define JSONRPC2HANDLER_CLASS

#include "mb_controller.hpp"
#include "JSONHandler/IRPC2CommandsObserver.h"
#include <json/value.h>
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/MessageQueue.h"
#include "Logger.hpp"

class JSONRPC2Handler : public NsMessageBroker::CMessageBrokerController
{
public:
    JSONRPC2Handler( const std::string& address, uint16_t port );
    ~JSONRPC2Handler();

    /*Methods from CMessageBrokerController*/
    void processResponse(std::string method, Json::Value& root);
    void processRequest(Json::Value& root);
    void processNotification(Json::Value& root);
    void subscribeToNotifications( );
    /*End of ethods from CMessageBrokerController*/

    /*Methods for IRPC2CommandsObserver*/
    void setRPC2CommandsObserver( IRPC2CommandsObserver * commandsObserver );
    void sendNotification( const RPC2Communication::RPC2Notification * command );
    void sendResponse( const RPC2Communication::RPC2Response * command );
    void sendRequest( const RPC2Communication::RPC2Request * command );
    //void sendCommand( const RPC2Communication::RPC2Command * command );
    /*End of methods for IRPC2CommandsObserver*/

private:
    static void * waitForCommandsFromHMI( void * params );
    static void * waitForResponsesFromHMI( void * params );
    static void * waitForResponsesToHMI( void * params );
    static void * waitForRequestsToHMI( void * params );
    static void * waitForNotificationsToHMI( void * params );

private:
    static log4cplus::Logger                              mLogger;
    struct ResponseContainer
    {
        std::string methodName;
        Json::Value response;
    };
    IRPC2CommandsObserver *                     mCommandsObserver;
    MessageQueue<Json::Value>                    mCommandsFromHMI;
    pthread_t                             mWaitForCommandsFromHMI;
    pthread_t                            mWaitForResponsesFromHMI;
    MessageQueue<ResponseContainer>             mResponsesFromHMI;
    //MessageQueue<const RPC2Communication::RPC2Command*>               mCommandsToHMI;
    MessageQueue<const RPC2Communication::RPC2Request*> mRequestsToHMI;
    MessageQueue<const RPC2Communication::RPC2Response*> mResponsesToHMI;
    MessageQueue<const RPC2Communication::RPC2Notification*> mNotificationsToHMI;
    pthread_t                               mWaitForRequestsToHMI;
    pthread_t                               mWaitForResponsesToHMI;
    pthread_t                               mWaitForNotificationsToHMI;
};

#endif
