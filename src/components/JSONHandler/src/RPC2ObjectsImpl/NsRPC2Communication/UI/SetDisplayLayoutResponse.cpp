#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetDisplayLayoutResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


SetDisplayLayoutResponse& SetDisplayLayoutResponse::operator =(const SetDisplayLayoutResponse& c)
{
  return *this;
}


SetDisplayLayoutResponse::~SetDisplayLayoutResponse(void)
{
}


SetDisplayLayoutResponse::SetDisplayLayoutResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE)
{
}


SetDisplayLayoutResponse::SetDisplayLayoutResponse(const SetDisplayLayoutResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SetDisplayLayoutResponse::checkIntegrity(void)
{
  return SetDisplayLayoutResponseMarshaller::checkIntegrity(*this);
}
