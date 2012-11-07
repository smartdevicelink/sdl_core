#include "../include/JSONHandler/RPC2Objects/UI/AddSubMenuResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


AddSubMenuResponse& AddSubMenuResponse::operator =(const AddSubMenuResponse& c)
{
  return *this;
}


AddSubMenuResponse::~AddSubMenuResponse(void)
{
}


AddSubMenuResponse::AddSubMenuResponse(void) : 
  RPC2Response(Marshaller::METHOD_ADDSUBMENURESPONSE)
{
}


AddSubMenuResponse::AddSubMenuResponse(const AddSubMenuResponse& c) : RPC2Response(Marshaller::METHOD_ADDSUBMENURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AddSubMenuResponse::checkIntegrity(void)
{
  return AddSubMenuResponseMarshaller::checkIntegrity(*this);
}
