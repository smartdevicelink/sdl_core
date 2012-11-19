#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/DeleteCommandResponse.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


DeleteCommandResponse& DeleteCommandResponse::operator =(const DeleteCommandResponse& c)
{
  return *this;
}


DeleteCommandResponse::~DeleteCommandResponse(void)
{
}


DeleteCommandResponse::DeleteCommandResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE)
{
}


DeleteCommandResponse::DeleteCommandResponse(const DeleteCommandResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteCommandResponse::checkIntegrity(void)
{
  return DeleteCommandResponseMarshaller::checkIntegrity(*this);
}
