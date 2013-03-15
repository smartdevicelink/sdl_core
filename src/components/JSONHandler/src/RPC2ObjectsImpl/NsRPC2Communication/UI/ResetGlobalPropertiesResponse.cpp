//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


ResetGlobalPropertiesResponse& ResetGlobalPropertiesResponse::operator =(const ResetGlobalPropertiesResponse& c)
{
  return *this;
}


ResetGlobalPropertiesResponse::~ResetGlobalPropertiesResponse(void)
{
}


ResetGlobalPropertiesResponse::ResetGlobalPropertiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE)
{
}


ResetGlobalPropertiesResponse::ResetGlobalPropertiesResponse(const ResetGlobalPropertiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ResetGlobalPropertiesResponse::checkIntegrity(void)
{
  return ResetGlobalPropertiesResponseMarshaller::checkIntegrity(*this);
}
