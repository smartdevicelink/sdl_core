
#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"

MobileRPCRequest::MobileRPCRequest( unsigned int protocolVersion, unsigned int correlationID )
:MobileRPCMessage( protocolVersion, REQUEST )
{}

MobileRPCRequest::~MobileRPCRequest() 
{}