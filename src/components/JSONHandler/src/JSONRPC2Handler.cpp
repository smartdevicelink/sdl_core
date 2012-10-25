#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/RPC2Marshaller.h"

log4cplus::Logger JSONRPC2Handler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("JSONRPC2Handler"));

JSONRPC2Handler::JSONRPC2Handler( const std::string& address, uint16_t port )
:NsMessageBroker::CMessageBrokerController( address, port, "AppLinkCore" )
{
    pthread_create( &mWaitForCommandsFromHMI, NULL, &JSONRPC2Handler::waitForCommandsFromHMI, (void *)this );
    pthread_create( &mWaitForCommandsToHMI, NULL, &JSONRPC2Handler::waitForCommandsToHMI, (void *)this );
    pthread_create( &mWaitForResponsesFromHMI, NULL, &JSONRPC2Handler::waitForResponsesFromHMI, (void *)this );
}

JSONRPC2Handler::~JSONRPC2Handler()
{
    /*pthread_kill( mWaitForCommandsFromHMI, 1 );
    pthread_kill( mWaitForCommandsToHMI, 1 );
    pthread_kill( mWaitForResponsesFromHMI, 1 );*/
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

void JSONRPC2Handler::setRPC2CommandsObserver( 
            IRPC2CommandsObserver * commandsObserver )
{
    mCommandsObserver = commandsObserver;
}

void JSONRPC2Handler::sendCommand( const RPC2Communication::RPC2Command * command )
{
    mCommandsToHMI.push( command );
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
                //TODO: log
            }

            if ( !handler -> mCommandsObserver )
            {
                //TODO: log
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
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForResponsesFromHMI: received response");
            ResponseContainer response = handler -> mResponsesFromHMI.pop();
            RPC2Communication::RPC2Command * currentCommand = RPC2Communication::RPC2Marshaller::fromJSON( response.response, response.methodName );

            if ( !currentCommand )
            {
                //TODO: log
            }
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForResponsesFromHMI: handle response");
            if ( !handler -> mCommandsObserver )
            {
                //TODO: log
                pthread_exit( 0 );
            }
            handler -> mCommandsObserver -> onCommandReceivedCallback( currentCommand );
        }
        handler -> mResponsesFromHMI.wait();
    }

    return 0;
}

void * JSONRPC2Handler::waitForCommandsToHMI( void * params )
{
    JSONRPC2Handler * handler = static_cast<JSONRPC2Handler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }
    LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI");
    while ( 1 )
    {
        while ( !handler -> mCommandsToHMI.empty() )
        {
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: received command.");
            const RPC2Communication::RPC2Command * command = handler -> mCommandsToHMI.pop();
            Json::Value commandJson = RPC2Communication::RPC2Marshaller::toJSON( command );

            if ( commandJson.isNull() )
            {
                //TODO: log
            }
            LOG4CPLUS_INFO(mLogger, "JSONRPC2Handler::waitForCommandsToHMI: processed command" );

            handler -> sendJsonMessage( commandJson );
        }
        handler -> mCommandsToHMI.wait();
    }

    return 0;
}
