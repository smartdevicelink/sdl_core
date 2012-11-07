#include "JSONHandler/ALRPCNotification.h"

using namespace AppLinkRPC;

ALRPCNotification::ALRPCNotification(void) :
    ALRPCMessage(0,NOTIFICATION)
{
}


ALRPCNotification::ALRPCNotification( unsigned int protocolVersion) :
    ALRPCMessage(protocolVersion,NOTIFICATION)
{
}

ALRPCNotification::ALRPCNotification( unsigned int protocolVersion, int methodId ) :
    ALRPCMessage(protocolVersion,NOTIFICATION, methodId)
{
}


ALRPCNotification::~ALRPCNotification()
{
}
