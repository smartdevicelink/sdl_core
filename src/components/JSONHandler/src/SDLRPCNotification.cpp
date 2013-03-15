/**
* \file SDLRPCNotification.cpp
* \brief SDLRPCNotification class source file.
* Copyright (c) 2013 Ford Motor Company
*/


#include "JSONHandler/SDLRPCNotification.h"

using namespace NsSmartDeviceLinkRPC;

SDLRPCNotification::SDLRPCNotification(void) :
SDLRPCMessage(0,NOTIFICATION)
{
}


SDLRPCNotification::SDLRPCNotification( unsigned int protocolVersion) :
SDLRPCMessage(protocolVersion,NOTIFICATION)
{
}

SDLRPCNotification::SDLRPCNotification( unsigned int protocolVersion, int methodId ) :
SDLRPCMessage(protocolVersion,NOTIFICATION, methodId)
{
}


SDLRPCNotification::~SDLRPCNotification()
{
}
