#include "../include/JSONHandler/ALRPCObjects/V2/ListFiles_request.h"
#include "ListFiles_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

ListFiles_request::~ListFiles_request(void)
{
}


ListFiles_request::ListFiles_request(const ListFiles_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool ListFiles_request::checkIntegrity(void)
{
  return ListFiles_requestMarshaller::checkIntegrity(*this);
}


ListFiles_request::ListFiles_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}






