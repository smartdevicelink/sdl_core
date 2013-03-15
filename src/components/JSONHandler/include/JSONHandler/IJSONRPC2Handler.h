/**
* \file IJSONRPC2Handler.h
* \brief IJSONRPC2Handler class header.
* Copyright (c) 2013 Ford Motor Company
*/

#ifndef IJSONRPC2HANDLER_INCLUDE
#define IJSONRPC2HANDLER_INCLUDE

#include "JSONHandler/IRPC2CommandsObserver.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Request.h"
/**
 * \class IJSONRPC2Handler
 * \brief Interface for making a bridge between Application Manager
 * and RPCBus (HMI).
*/
class IJSONRPC2Handler
{
public:
    /**
     * \brief Destructor
    */
    virtual ~IJSONRPC2Handler() {}

    /**
     * \brief Notifies RPCBus of the will to receive specific notifications.
     */
    virtual void subscribeToNotifications( ) = 0;

    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (App Manager).
     * \param commandsObserver Pointer to implementation of IRPC2CommandsObserver.
     * \sa IRPC2CommandsObserver.
     */
    virtual void setRPC2CommandsObserver( IRPC2CommandsObserver * commandsObserver ) = 0;

    /**
     * \brief Sends notification to RPCBus.
     * \param command Json RPC2 notification to be sent to RPCBus.
     */
    virtual void sendNotification( const NsRPC2Communication::RPC2Notification * command ) = 0;

    /**
     * \brief Sends response to RPCBus.
     * \param command Json RPC2 response to be sent to RPCBus.
     */
    virtual void sendResponse( const NsRPC2Communication::RPC2Response * command ) = 0;

    /**
     * \brief Sends request to RPCBus.
     * \param command Json RPC2 request to be sent to RPCBus.
     */
    virtual void sendRequest( const NsRPC2Communication::RPC2Request * command ) = 0;
};

#endif // IJSONRPC2HANDLER_INCLUDE
