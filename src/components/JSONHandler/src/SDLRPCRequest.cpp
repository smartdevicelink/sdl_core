/**
* \file SDLRPCRequest.cpp
* \brief SDLRPCRequest class source file.
* Copyright (c) 2013 Ford Motor Company
*/


#include "JSONHandler/SDLRPCRequest.h"

using namespace NsSmartDeviceLinkRPC;

SDLRPCRequest::SDLRPCRequest( unsigned int protocolVersion ) :
SDLRPCMessage( protocolVersion, REQUEST )
, mCorrelationID(0)
{}

SDLRPCRequest::SDLRPCRequest(void) :
SDLRPCMessage( 0, REQUEST )
, mCorrelationID(0)
{}

SDLRPCRequest::SDLRPCRequest( unsigned int protocolVersion,  int methodId ) :
SDLRPCMessage( protocolVersion, REQUEST, methodId)
, mCorrelationID(0)
{}

SDLRPCRequest::~SDLRPCRequest()
{}

unsigned int SDLRPCRequest::getCorrelationID() const
{
    return mCorrelationID;
}

void SDLRPCRequest::setCorrelationID( unsigned int correlationID )
{
    mCorrelationID = correlationID;
}
