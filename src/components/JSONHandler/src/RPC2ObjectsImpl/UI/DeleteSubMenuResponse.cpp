#include "../include/JSONHandler/RPC2Objects/UI/DeleteSubMenuResponse.h"
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


DeleteSubMenuResponse& DeleteSubMenuResponse::operator =(const DeleteSubMenuResponse& c)
{
  return *this;
}


DeleteSubMenuResponse::~DeleteSubMenuResponse(void)
{
}


DeleteSubMenuResponse::DeleteSubMenuResponse(void) : 
  RPC2Response(Marshaller::METHOD_DELETESUBMENURESPONSE)
{
}


DeleteSubMenuResponse::DeleteSubMenuResponse(const DeleteSubMenuResponse& c) : RPC2Response(Marshaller::METHOD_DELETESUBMENURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteSubMenuResponse::checkIntegrity(void)
{
  return DeleteSubMenuResponseMarshaller::checkIntegrity(*this);
}
