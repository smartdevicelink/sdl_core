#include "JSONHandler/JSONRPC2Handler.h"

JSONRPC2Handler::JSONRPC2Handler( const std::string& address, uint16_t port )
:NsMessageBroker::CMessageBrokerController( address, port, "" )
,mCommandsMarshaller(0)
{
    pthread_create( &mWaitForCommandsFromHMI, NULL, &JSONRPC2Handler::waitForCommandsFromHMI, (void *)this );
    pthread_create( &mWaitForCommandsToHMI, NULL, &JSONRPC2Handler::waitForCommandsToHMI, (void *)this );
    pthread_create( &mWaitForResponsesFromHMI, NULL, &JSONRPC2Handler::waitForResponsesFromHMI, (void *)this );
    mCommandsMarshaller = new RPC2Marshaller;
}

JSONRPC2Handler::~JSONRPC2Handler()
{
    /*pthread_kill( mWaitForCommandsFromHMI, 1 );
    pthread_kill( mWaitForCommandsToHMI, 1 );
    pthread_kill( mWaitForResponsesFromHMI, 1 );*/
    delete mCommandsMarshaller;
    mCommandsMarshaller = 0;
}

void JSONRPC2Handler::processResponse(std::string method, Json::Value& root)
{
    ResponseContainer response;
    response.methodName = method;
    response.response = root;
    mResponsesFromHMI.push( response );
}
    
void JSONRPC2Handler::processRequest(Json::Value& root)
{
    mCommandsFromHMI.push( root );
}
    
void JSONRPC2Handler::processNotification(Json::Value& root)
{
    mCommandsFromHMI.push( root );
}

void JSONRPC2Handler::setRPC2CommandsObserver( 
            IRPC2CommandsObserver * commandsObserver )
{
    mCommandsObserver = commandsObserver;
}

void JSONRPC2Handler::sendCommand( const RPC2Command * command )
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

    while ( 1 )
    {
        while ( !handler -> mCommandsFromHMI.empty() )
        {
            Json::Value jsonMessage = handler -> mCommandsFromHMI.pop();
            RPC2Command * currentCommand = RPC2Marshaller::fromJSON( jsonMessage );

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

    while ( 1 )
    {
        while ( !handler -> mResponsesFromHMI.empty() )
        {
            ResponseContainer response = handler -> mResponsesFromHMI.pop();
            RPC2Command * currentCommand = RPC2Marshaller::fromJSON( response.response, response.methodName );

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

    while ( 1 )
    {
        while ( !handler -> mCommandsToHMI.empty() )
        {
            const RPC2Command * command = handler -> mCommandsToHMI.pop();
            Json::Value commandJson = RPC2Marshaller::toJSON( command );

            if ( commandJson.isNull() )
            {
                //TODO: log
            }

            handler -> sendJsonMessage( commandJson );
        }
        handler -> mCommandsToHMI.wait();
    }

    return 0;
}
