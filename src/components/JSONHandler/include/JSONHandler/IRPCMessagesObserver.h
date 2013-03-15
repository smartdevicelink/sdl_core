/**
* \file IRPCMessagesObserver.hpp
* \brief Abstract class IRPCMessagesObserver header.
* Copyright (c) 2013 Ford Motor Company
*/


#ifndef IRPCMESSAGES_OBSERVER_CLASS
#define IRPCMESSAGES_OBSERVER_CLASS 

#include "JSONHandler/SDLRPCMessage.h"

/**
 * \class IRPCMessagesObserver
 * \brief Abstract class for communication with Protocol layer. 
 * Descendant class has to implement onMessageReceivedCallback
 * \sa onMessageReceivedCallback
*/
class IRPCMessagesObserver
{
public:
    /**
     * \brief Callback function which is called by JSONHandler 
     * when new SmartDeviceLink Json message is received from Mobile Application.
     * \param message SmartDeviceLink Json message
     */
    virtual void onMessageReceivedCallback( NsSmartDeviceLinkRPC::SDLRPCMessage * message, int connectionKey ) = 0;

protected:
    /**
     * \brief Destructor
    */
    virtual ~IRPCMessagesObserver() {};

};

#endif  //  IRPCMESSAGES_OBSERVER_CLASS
