#include "../include/JSONHandler/ALRPCObjects/V1/UnregisterAppInterface_request.h"
#include "UnregisterAppInterface_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;

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






