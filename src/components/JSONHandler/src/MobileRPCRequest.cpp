
#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"

MobileRPCRequest::MobileRPCRequest( unsigned int protocolVersion )
:MobileRPCMessage( protocolVersion, REQUEST )
{}

MobileRPCRequest::MobileRPCRequest( unsigned int protocolVersion, std::string functionName )
:MobileRPCMessage( protocolVersion, REQUEST, functionName )
{}
    
MobileRPCRequest::MobileRPCRequest( unsigned int protocolVersion, unsigned int functionID )
:MobileRPCMessage( protocolVersion, REQUEST, functionID )
{}
 
MobileRPCRequest::~MobileRPCRequest() 
{}