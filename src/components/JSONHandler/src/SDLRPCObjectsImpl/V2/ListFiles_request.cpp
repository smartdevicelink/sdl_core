//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/ListFiles_request.h"
#include "ListFiles_requestMarshaller.h"
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

ListFiles_request::~ListFiles_request(void)
{
}


ListFiles_request::ListFiles_request(const ListFiles_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool ListFiles_request::checkIntegrity(void)
{
  return ListFiles_requestMarshaller::checkIntegrity(*this);
}


ListFiles_request::ListFiles_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}






