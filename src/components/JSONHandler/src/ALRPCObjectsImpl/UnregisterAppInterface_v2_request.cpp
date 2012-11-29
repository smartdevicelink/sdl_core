#include "../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_v2_request.h"
#include "UnregisterAppInterface_v2_requestMarshaller.h"
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

UnregisterAppInterface_v2_request::~UnregisterAppInterface_v2_request(void)
{
}


UnregisterAppInterface_v2_request::UnregisterAppInterface_v2_request(const UnregisterAppInterface_v2_request& c)
{
  *this=c;
}


bool UnregisterAppInterface_v2_request::checkIntegrity(void)
{
  return UnregisterAppInterface_v2_requestMarshaller::checkIntegrity(*this);
}


UnregisterAppInterface_v2_request::UnregisterAppInterface_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_UNREGISTERAPPINTERFACE_V2_REQUEST)
{
}






