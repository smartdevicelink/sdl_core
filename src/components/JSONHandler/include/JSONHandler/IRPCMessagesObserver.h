#ifndef IRPCMESSAGES_OBSERVER_CLASS
#define IRPCMESSAGES_OBSERVER_CLASS value

#include "JSONHandler/ALRPCMessage.h"

class IRPCMessagesObserver
{
public:
    virtual void onMessageReceivedCallback( ALRPCMessage * message ) = 0;

protected:
    virtual ~IRPCMessagesObserver() {};

};

#endif