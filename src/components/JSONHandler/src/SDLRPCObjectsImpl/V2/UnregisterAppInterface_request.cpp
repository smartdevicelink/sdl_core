//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/UnregisterAppInterface_request.h"
#include "UnregisterAppInterface_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

UnregisterAppInterface_request::~UnregisterAppInterface_request(void)
{
}


UnregisterAppInterface_request::UnregisterAppInterface_request(const UnregisterAppInterface_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool UnregisterAppInterface_request::checkIntegrity(void)
{
  return UnregisterAppInterface_requestMarshaller::checkIntegrity(*this);
}


UnregisterAppInterface_request::UnregisterAppInterface_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}






