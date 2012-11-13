#ifndef IRPCMESSAGES_OBSERVER_CLASS
#define IRPCMESSAGES_OBSERVER_CLASS value

#include "JSONHandler/ALRPCMessage.h"

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
     * when new AppLink Json message is received from Mobile Application.
     * \param message AppLink Json message
     */
    virtual void onMessageReceivedCallback( AppLinkRPC::ALRPCMessage * message, unsigned char sessionID ) = 0;

protected:
    /**
     * \brief Destructor
    */
    virtual ~IRPCMessagesObserver() {};

};

#endif