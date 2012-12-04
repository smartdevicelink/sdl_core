#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


DeleteSubMenuResponse& DeleteSubMenuResponse::operator =(const DeleteSubMenuResponse& c)
{
  return *this;
}


DeleteSubMenuResponse::~DeleteSubMenuResponse(void)
{
}


DeleteSubMenuResponse::DeleteSubMenuResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE)
{
}


DeleteSubMenuResponse::DeleteSubMenuResponse(const DeleteSubMenuResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteSubMenuResponse::checkIntegrity(void)
{
  return DeleteSubMenuResponseMarshaller::checkIntegrity(*this);
}
