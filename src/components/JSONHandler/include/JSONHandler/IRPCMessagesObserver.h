#ifndef IRPCMESSAGES_OBSERVER_CLASS
#define IRPCMESSAGES_OBSERVER_CLASS value

#include "JSONHandler/MobileRPCMessage.h"

class IRPCMessagesObserver
{
public:
    virtual void onMessageReceivedCallback( MobileRPCMessage * message ) = 0;

protected:
    virtual ~IRPCMessagesObserver() {};

};

#endif