#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/RPC2Marshaller.h"

log4cplus::Logger JSONRPC2Handler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("JSONRPC2Handler"));

JSONRPC2Handler::JSONRPC2Handler( const std::string& address, uint16_t port )
:NsMessageBroker::CMessageBrokerController( address, port, "AppLinkCore" )
{
    pthread_create( &mWaitForCommandsFromHMI, NULL, &JSONRPC2Handler::waitForCommandsFromHMI, (void *)this );
    pthread_create( &mWaitForRequestsToHMI, NULL, &JSONRPC2Handler::waitForRequestsToHMI, (void *)this );
    pthread_create( &mWaitForResponsesToHMI, NULL, &JSONRPC2Handler::waitForResponsesToHMI, (void *)this );
    pthread_create( &mWaitForNotificationsToHMI, NULL, &JSONRPC2Handler::waitForNotificationsToHMI, (void *)this );
    pthread_create( &mWaitForResponsesFromHMI, NULL, &JSONRPC2Handler::waitForResponsesFromHMI, (void *)this );
}

JSONRPC2Handler::~JSONRPC2Handler()
{
}

void JSONRPC2Handler::processResponse(std::string method, Json::Value& root)
{
    LOG4CPLUS_INFO( mLogger, "Received response from RPCBus" );
    ResponseContainer response;
    response.methodName = method;
    response.response = root;
    mResponsesFromHMI.push( response );
}
    
void JSONRPC2Handler::processRequest(Json::Value& root)
{
    LOG4CPLUS_INFO(mLogger, "Received request from RPCBus");
    mCommandsFromHMI.push( root );
}
    
void JSONRPC2Handler::processNotification(Json::Value& root)
{
    LOG4CPLUS_INFO( mLogger, "Received notification from RPCBus." );
    mCommandsFromHMI.push( root );
}

void JSONRPC2Handler::subscribeToNotifications()
{
    subscribeTo( "Buttons.OnButtonEvent" );
    subscribeTo( "Buttons.OnButtonPress" );
    subscribeTo( "UI.OnCommand" );
}

void JSONRPC2Handler::setRPC2CommandsObserver( 
            IRPC2CommandsObserver * commandsObserver )
{
    mCommandsObserver = commandsObserver;
}

/*void JSONRPC2Handler::sendCommand( const RPC2Communication::RPC2Command * command )
{
    mCommandsToHMI.push( command );
}*/

void JSONRPC2Handler::sendNotification( const RPC2Communication::RPC2Notification * command )
{
    mNotificationsToHMI.push( command );
}
    
void JSONRPC2Handler::sendResponse( const RPC2Communication::RPC2Response * command )
{
    mResponsesToHMI.push( command );
}
    
void JSONRPC2Handler::sendRequest( const RPC2Communication::RPC2Request * command )
{
    mRequestsToHMI.push( command );
}

void * JSONRPC2Handler::waitForCommandsFromHMI( void * params )
{
    JSONRPC2Handler * handler = static_cast<JSONRPC2Handler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }
    LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsFromHMI");
    while ( 1 )
    {
        while ( !handler -> mCommandsFromHMI.empty() )
        {
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsFromHMI: received command");
            Json::Value jsonMessage = handler -> mCommandsFromHMI.pop();
            RPC2Communication::RPC2Command * currentCommand = RPC2Communication::RPC2Marshaller::fromJSON( jsonMessage );
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsFromHMI: handle command" );
            if ( !currentCommand )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid message received." );
                continue;
            }

            if ( !handler -> mCommandsObserver )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle message: CommandsObserver doesn't exist." );
                pthread_exit( 0 );
            }
            handler -> mCommandsObserver -> onCommandReceivedCallback( currentCommand );
        }
        handler -> mCommandsFromHMI.wait();
    }

    return 0;
}

void * JSONRPC2Handler::waitForResponsesFromHMI( void * params )
{
    JSONRPC2Handler * handler = static_cast<JSONRPC2Handler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }
    LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForResponsesFromHMI\n");
    while ( 1 )
    {
        while ( !handler -> mResponsesFromHMI.empty() )
        {            
            ResponseContainer response = handler -> mResponsesFromHMI.pop();
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForResponsesFromHMI: received response " << response.methodName );
            RPC2Communication::RPC2Command * currentCommand = RPC2Communication::RPC2Marshaller::fromJSON( response.response, response.methodName );

            if ( !currentCommand )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid message received." );
                continue;
            }
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForResponsesFromHMI: handle response");
            if ( !handler -> mCommandsObserver )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle message: CommandsObserver doesn't exist." );
                pthread_exit( 0 );
            }
            handler -> mCommandsObserver -> onCommandReceivedCallback( currentCommand );
        }
        handler -> mResponsesFromHMI.wait();
    }

    return 0;
}

void * JSONRPC2Handler::waitForResponsesToHMI( void * params )
{
    JSONRPC2Handler * handler = static_cast<JSONRPC2Handler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }
    LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI");
    while ( 1 )
    {
        while ( !handler -> mResponsesToHMI.empty() )
        {            
            const RPC2Communication::RPC2Response * command = handler -> mResponsesToHMI.pop();
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: received command " << command->getID());
            Json::Value commandJson = RPC2Communication::RPC2Marshaller::toJSON( command );

            if ( commandJson.isNull() )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid message received." );
            }
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: processed command" );

            handler -> prepareMessage( commandJson );
            handler -> sendJsonMessage( commandJson );

            delete command;
        }
        handler -> mResponsesToHMI.wait();
    }

    return 0;
}
    
void * JSONRPC2Handler::waitForRequestsToHMI( void * params )
{
    JSONRPC2Handler * handler = static_cast<JSONRPC2Handler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }
    LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI");
    while ( 1 )
    {
        while ( !handler -> mRequestsToHMI.empty() )
        {
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: received command.");
            const RPC2Communication::RPC2Command * command = handler -> mRequestsToHMI.pop();
            Json::Value commandJson = RPC2Communication::RPC2Marshaller::toJSON( command );

            if ( commandJson.isNull() )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid message received." );
            }
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: processed command" );

            handler -> prepareMessage( commandJson );
            handler -> sendJsonMessage( commandJson );

            delete command;
        }
        handler -> mRequestsToHMI.wait();
    }

    return 0;
}
    
void * JSONRPC2Handler::waitForNotificationsToHMI( void * params )
{
    JSONRPC2Handler * handler = static_cast<JSONRPC2Handler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }
    LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI");
    while ( 1 )
    {
        while ( !handler -> mNotificationsToHMI.empty() )
        {
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: received command.");
            const RPC2Communication::RPC2Command * command = handler -> mNotificationsToHMI.pop();
            Json::Value commandJson = RPC2Communication::RPC2Marshaller::toJSON( command );

            if ( commandJson.isNull() )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid message received." );
            }
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: processed command" );

            handler -> sendJsonMessage( commandJson );

            delete command;
        }
        handler -> mNotificationsToHMI.wait();
    }

    return 0;
}
