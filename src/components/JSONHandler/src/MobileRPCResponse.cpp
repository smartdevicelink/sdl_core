#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCResponse.h"

MobileRPCResponse::MobileRPCResponse( unsigned int protocolVersion )
:MobileRPCMessage( protocolVersion, RESPONSE )
{}

MobileRPCResponse::MobileRPCResponse( unsigned int protocolVersion, std::string functionName )
:MobileRPCMessage( protocolVersion, RESPONSE, functionName )
{}

MobileRPCResponse::MobileRPCResponse( unsigned int protocolVersion, unsigned int functionID )
:MobileRPCMessage( protocolVersion, RESPONSE, functionID )
{}

MobileRPCResponse::~MobileRPCResponse()
{}