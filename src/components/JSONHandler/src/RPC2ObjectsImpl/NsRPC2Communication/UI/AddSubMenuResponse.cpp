#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


AddSubMenuResponse& AddSubMenuResponse::operator =(const AddSubMenuResponse& c)
{
  return *this;
}


AddSubMenuResponse::~AddSubMenuResponse(void)
{
}


AddSubMenuResponse::AddSubMenuResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE)
{
}


AddSubMenuResponse::AddSubMenuResponse(const AddSubMenuResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AddSubMenuResponse::checkIntegrity(void)
{
  return AddSubMenuResponseMarshaller::checkIntegrity(*this);
}
