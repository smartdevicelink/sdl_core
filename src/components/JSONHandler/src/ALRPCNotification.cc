
#include "ALRPCNotification.hh"


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
