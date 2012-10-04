
#include "MobileRPCMessage.h"
#include "MobileRPCRequest.h"

MobileRPCRequest::MobileRPCRequest( unsigned int protocolVersion, unsigned int correlationID )
:MobileRPCMessage( protocolVersion, REQUEST, correlationID )
{}

