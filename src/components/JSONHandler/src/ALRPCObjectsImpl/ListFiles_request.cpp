#include "../../include/JSONHandler/ALRPCObjects/ListFiles_request.h"
#include "ListFiles_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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






