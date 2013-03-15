//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


SetGlobalPropertiesResponse& SetGlobalPropertiesResponse::operator =(const SetGlobalPropertiesResponse& c)
{
  return *this;
}


SetGlobalPropertiesResponse::~SetGlobalPropertiesResponse(void)
{
}


SetGlobalPropertiesResponse::SetGlobalPropertiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE)
{
}


SetGlobalPropertiesResponse::SetGlobalPropertiesResponse(const SetGlobalPropertiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SetGlobalPropertiesResponse::checkIntegrity(void)
{
  return SetGlobalPropertiesResponseMarshaller::checkIntegrity(*this);
}
