/**
* \file JSONRPC2Handler.cpp
* \brief JSONRPC2Handler class source file.
* Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
*/


#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"

log4cxx::LoggerPtr JSONRPC2Handler::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("JSONRPC2Handler"));

JSONRPC2Handler::JSONRPC2Handler( const std::string& address, uint16_t port ) :
NsMessageBroker::CMessageBrokerController( address, port, "BasicCommunication" )
{
  // TODO(AK): Are we need this threads?
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
    LOG4CXX_INFO_EXT( logger_, "Received response from RPCBus" );
    ResponseContainer response;
    response.methodName = method;
    response.response = root;
    mResponsesFromHMI.push( response );
}
    
void JSONRPC2Handler::processRequest(Json::Value& root)
{
    LOG4CXX_INFO_EXT(logger_, "Received request from RPCBus");
    mCommandsFromHMI.push( root );
}
    
void JSONRPC2Handler::processNotification(Json::Value& root)
{
    LOG4CXX_INFO_EXT( logger_, "Received notification from RPCBus." );
    mCommandsFromHMI.push( root );
}

void JSONRPC2Handler::subscribeToNotifications()
{
    subscribeTo( "Buttons.OnButtonEvent" );
    subscribeTo( "Buttons.OnButtonPress" );
    subscribeTo( "UI.OnCommand" );
    subscribeTo( "VR.OnCommand" );
    subscribeTo( "UI.OnReady" );
    subscribeTo( "UI.OnDriverDistraction" );
    subscribeTo( "UI.OnSystemContext" );
    subscribeTo( "UI.OnAppActivated" );
    subscribeTo( "BasicCommunication.OnAppDeactivated" );
    subscribeTo( "UI.OnDeviceChosen");
    subscribeTo( "UI.OnLanguageChange" );
    subscribeTo( "VR.OnLanguageChange" );
    subscribeTo( "TTS.OnLanguageChange" );
    subscribeTo( "VehicleInfo.OnVehicleData" );
    subscribeTo( "UI.OnTBTClientState" );
}

void JSONRPC2Handler::setRPC2CommandsObserver( 
            IRPC2CommandsObserver * commandsObserver )
{
    mCommandsObserver = commandsObserver;
}

void JSONRPC2Handler::sendNotification( const NsRPC2Communication::RPC2Notification * command )
{
    mNotificationsToHMI.push( command );
}
    
void JSONRPC2Handler::sendResponse( const NsRPC2Communication::RPC2Response * command )
{
    mResponsesToHMI.push( command );
}
    
void JSONRPC2Handler::sendRequest( const NsRPC2Communication::RPC2Request * command )
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
    LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForCommandsFromHMI");
    while ( 1 )
    {
        while ( !handler -> mCommandsFromHMI.empty() )
        {
            Json::Value jsonMessage = handler -> mCommandsFromHMI.pop();
            LOG4CXX_INFO_EXT(logger_, "JSONRPC2Handler::waitForCommandsFromHMI: received command: " << jsonMessage);
            NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( jsonMessage );
            
            if ( !currentCommand )
            {
                LOG4CXX_ERROR( logger_, "Invalid RPCBus message received." );
                continue;
            }
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForCommandsFromHMI: handle command" );

            if ( !handler -> mCommandsObserver )
            {
                LOG4CXX_ERROR( logger_, "Cannot handle RPCBus message: CommandsObserver doesn't exist." );
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
    LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForResponsesFromHMI\n");
    while ( 1 )
    {
        while ( !handler -> mResponsesFromHMI.empty() )
        {            
            ResponseContainer response = handler -> mResponsesFromHMI.pop();
            LOG4CXX_INFO_EXT(logger_, "JSONRPC2Handler::waitForResponsesFromHMI: received response to a method " << response.methodName << "\n" << response.response );
            NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( response.response );
            
            if ( !currentCommand )
            {
                LOG4CXX_ERROR( logger_, "Invalid RPCBus message received." );
                continue;
            }
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForResponsesFromHMI: handle response");
            if ( !handler -> mCommandsObserver )
            {
                LOG4CXX_ERROR( logger_, "Cannot handle RPCBus message: CommandsObserver doesn't exist." );
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
    LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForResponsesToHMI");
    while ( 1 )
    {
        while ( !handler -> mResponsesToHMI.empty() )
        {            
            const NsRPC2Communication::RPC2Response * command = handler -> mResponsesToHMI.pop();

            Json::Value commandJson = NsRPC2Communication::Marshaller::toJSON( command );
            LOG4CXX_INFO_EXT(logger_, "JSONRPC2Handler::waitForResponsesToHMI: received response " << command->getId() << commandJson);

            if ( commandJson.isNull() )
            {
                LOG4CXX_ERROR( logger_, "Invalid RPCBus message received." );
                continue;
            }
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForResponsesToHMI: processed command" );

            //handler -> prepareMessage( commandJson );
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
    LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForRequestsToHMI");
    while ( 1 )
    {
        while ( !handler -> mRequestsToHMI.empty() )
        {
            const NsRPC2Communication::RPC2Command * command = handler -> mRequestsToHMI.pop();
            Json::Value commandJson = NsRPC2Communication::Marshaller::toJSON( command );
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForRequestsToHMI: received request " << commandJson);

            if ( commandJson.isNull() )
            {
                LOG4CXX_ERROR( logger_, "Invalid RPCBus message received." );
                continue;
            }
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForRequestsToHMI: processed command" );

            //handler -> prepareMessage( commandJson );
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
    LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForNotificationsToHMI");
    while ( 1 )
    {
        while ( !handler -> mNotificationsToHMI.empty() )
        {
            const NsRPC2Communication::RPC2Command * command = handler -> mNotificationsToHMI.pop();
            Json::Value commandJson = NsRPC2Communication::Marshaller::toJSON( command );
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForNotificationsToHMI: received notification " << commandJson);

            if ( commandJson.isNull() )
            {
                LOG4CXX_ERROR( logger_, "Invalid RPCBus message received." );
                continue;
            }
            LOG4CXX_INFO(logger_, "JSONRPC2Handler::waitForNotificationsToHMI: processed command" );

            handler -> sendJsonMessage( commandJson );

            delete command;
        }
        handler -> mNotificationsToHMI.wait();
    }

    return 0;
}
