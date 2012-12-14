#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/UpdateTurnListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;


UpdateTurnListResponse& UpdateTurnListResponse::operator =(const UpdateTurnListResponse& c)
{
  return *this;
}


UpdateTurnListResponse::~UpdateTurnListResponse(void)
{
}


UpdateTurnListResponse::UpdateTurnListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLISTRESPONSE)
{
}


UpdateTurnListResponse::UpdateTurnListResponse(const UpdateTurnListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLISTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool UpdateTurnListResponse::checkIntegrity(void)
{
  return UpdateTurnListResponseMarshaller::checkIntegrity(*this);
}
