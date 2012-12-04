#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
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
