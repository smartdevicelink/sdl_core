#include "../include/JSONHandler/ALRPCObjects/ListFiles_request.h"
#include "ListFiles_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

ListFiles_request::~ListFiles_request(void)
{
}


ListFiles_request::ListFiles_request(const ListFiles_request& c)
{
  *this=c;
}


bool ListFiles_request::checkIntegrity(void)
{
  return ListFiles_requestMarshaller::checkIntegrity(*this);
}


ListFiles_request::ListFiles_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_LISTFILES_REQUEST)
{
}






