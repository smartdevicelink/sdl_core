#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


DeleteCommandResponse& DeleteCommandResponse::operator =(const DeleteCommandResponse& c)
{
  return *this;
}


DeleteCommandResponse::~DeleteCommandResponse(void)
{
}


DeleteCommandResponse::DeleteCommandResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE)
{
}


DeleteCommandResponse::DeleteCommandResponse(const DeleteCommandResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteCommandResponse::checkIntegrity(void)
{
  return DeleteCommandResponseMarshaller::checkIntegrity(*this);
}
