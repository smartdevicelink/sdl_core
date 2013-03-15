/**
* \file IJsonHandler.h
* \brief IJsonHandler class header.
* Copyright (c) 2013 Ford Motor Company
*/

#ifndef IJSONHANDLER_INCLUDE
#define IJSONHANDLER_INCLUDE

#include "ProtocolHandler/ProtocolHandler.h"

/**
 * \class IJsonHandler
 * \brief Interface for making a bridge between 
 * ProtocolHandler and Application Manager.
*/
class IJsonHandler
{
public:
    /**
     * \brief Destructor
    */
    virtual ~IJsonHandler() {}
    /**
     * \brief Sets pointer for Protocol layer handler for message exchange.
     * \param protocolHandler Pointer to Protocol layer handler.
     */
    virtual void setProtocolHandler( NsProtocolHandler::ProtocolHandler * protocolHandler ) = 0;

    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
     * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
     * \sa IRPCMessagesObserver
     */
    virtual void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver ) = 0;

    /**
     * \brief Method for sending message to Mobile Application. 
     * \param message Pointer to base class of SmartDeviceLink Json object
     * to be serialized to Json message and sent to mobile App.
     * \param sessionId ID of the session the message was received within.
     */
    virtual void sendRPCMessage( const NsSmartDeviceLinkRPC::SDLRPCMessage * message, int connectionKey ) = 0;
    
};

#endif // IJSONHANDLER_INCLUDE
