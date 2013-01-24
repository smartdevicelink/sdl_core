#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/UpdateTurnListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


UpdateTurnListResponse& UpdateTurnListResponse::operator =(const UpdateTurnListResponse& c)
{
  return *this;
}


UpdateTurnListResponse::~UpdateTurnListResponse(void)
{
}


UpdateTurnListResponse::UpdateTurnListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__UPDATETURNLISTRESPONSE)
{
}


UpdateTurnListResponse::UpdateTurnListResponse(const UpdateTurnListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__UPDATETURNLISTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool UpdateTurnListResponse::checkIntegrity(void)
{
  return UpdateTurnListResponseMarshaller::checkIntegrity(*this);
}
