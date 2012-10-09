#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCNotification.h"

MobileRPCNotification::MobileRPCNotification( unsigned int protocolVersion )
:MobileRPCMessage( protocolVersion, NOTIFICATION )
{}
 
MobileRPCNotification::MobileRPCNotification( unsigned int protocolVersion, std::string functionName )
:MobileRPCMessage( protocolVersion, NOTIFICATION, functionName )
{}
 
MobileRPCNotification::MobileRPCNotification( unsigned int protocolVersion, unsigned int functionID )
:MobileRPCMessage( protocolVersion, NOTIFICATION, functionID )
{}
 
MobileRPCNotification::~MobileRPCNotification()
{}