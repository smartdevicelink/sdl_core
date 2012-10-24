#include "ALRPCRequest.hh"

ALRPCRequest::ALRPCRequest( unsigned int protocolVersion )
:ALRPCMessage( protocolVersion, REQUEST ) 
, mCorrelationID(0)
{}

ALRPCRequest::ALRPCRequest(void)
:ALRPCMessage( 0, REQUEST ) 
, mCorrelationID(0)
{}

ALRPCRequest::ALRPCRequest( unsigned int protocolVersion,  int methodId )
:ALRPCMessage( protocolVersion, REQUEST, methodId)
, mCorrelationID(0)
{}

ALRPCRequest::~ALRPCRequest() 
{}

unsigned int ALRPCRequest::getCorrelationID() const 
{ 
    return mCorrelationID; 
}

void ALRPCRequest::setCorrelationID( unsigned int correlationID ) 
{ 
    mCorrelationID = correlationID; 
}

