#include "../../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_request.h"
#include "UnregisterAppInterface_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


UnregisterAppInterface_request::~UnregisterAppInterface_request(void)
{
}


UnregisterAppInterface_request::UnregisterAppInterface_request(const UnregisterAppInterface_request& c)
{
  *this=c;
}


bool UnregisterAppInterface_request::checkIntegrity(void)
{
  return UnregisterAppInterface_requestMarshaller::checkIntegrity(*this);
}


UnregisterAppInterface_request::UnregisterAppInterface_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_UNREGISTERAPPINTERFACE_REQUEST)
{
}






