#include "../include/JSONHandler/RPC2Objects/UI/DeleteCommandResponse.h"
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


DeleteCommandResponse& DeleteCommandResponse::operator =(const DeleteCommandResponse& c)
{
  return *this;
}


DeleteCommandResponse::~DeleteCommandResponse(void)
{
}


DeleteCommandResponse::DeleteCommandResponse(void) : 
  RPC2Response(Marshaller::METHOD_DELETECOMMANDRESPONSE)
{
}


DeleteCommandResponse::DeleteCommandResponse(const DeleteCommandResponse& c) : RPC2Response(Marshaller::METHOD_DELETECOMMANDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteCommandResponse::checkIntegrity(void)
{
  return DeleteCommandResponseMarshaller::checkIntegrity(*this);
}
