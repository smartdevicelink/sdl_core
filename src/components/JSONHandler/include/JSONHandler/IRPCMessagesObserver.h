#ifndef IRPCMESSAGES_OBSERVER_CLASS
#define IRPCMESSAGES_OBSERVER_CLASS value

#include "JSONHandler/ALRPCMessage.h"

class IRPCMessagesObserver
{
public:
    virtual void onMessageReceivedCallback( AppLinkRPC::ALRPCMessage * message, unsigned char sessionID ) = 0;

protected:
    virtual ~IRPCMessagesObserver() {};

};

#endif