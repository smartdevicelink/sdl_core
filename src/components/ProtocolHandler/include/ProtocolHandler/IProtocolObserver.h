/**
* \file IProtocolObserver.h
* \brief IProtocolObserver class header.
* 
* Copyright (c) 2013 Ford Motor Company
*/

#ifndef IPROTOCOLOBSERVER_CLASS
#define IPROTOCOLOBSERVER_CLASS 

/**
  *\namespace NsProtocolHandler
  *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
*/
namespace NsProtocolHandler
{
    class SmartDeviceLinkRawMessage;
    
    /**
     * \class IProtocolObserver
     * \brief Interface for making a bridge between
     * ProtocolHandler and JSONHandler components.
    */
    class IProtocolObserver
    {
    public:    
        
        /**
         * \brief Callback function which is used by ProtocolHandler
         * when new message is received from Mobile Application. 
         * \param message Message with supporting params received
         */
        virtual void onDataReceivedCallback( const SmartDeviceLinkRawMessage * message ) = 0;

    protected:
        /**
         * \brief Destructor
        */
        virtual ~IProtocolObserver() {}

    };
}

#endif //IPROTOCOLOBSERVER_CLASS