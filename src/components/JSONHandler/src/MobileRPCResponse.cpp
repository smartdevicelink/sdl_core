#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCResponse.h"

MobileRPCResponse::MobileRPCResponse( unsigned int protocolVersion, unsigned int correlationID )
:MobileRPCMessage( protocolVersion, RESPONSE, correlationID )
{}