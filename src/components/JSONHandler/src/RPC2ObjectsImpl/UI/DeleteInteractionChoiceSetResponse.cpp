#include "../include/JSONHandler/RPC2Objects/UI/DeleteInteractionChoiceSetResponse.h"
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


DeleteInteractionChoiceSetResponse& DeleteInteractionChoiceSetResponse::operator =(const DeleteInteractionChoiceSetResponse& c)
{
  return *this;
}


DeleteInteractionChoiceSetResponse::~DeleteInteractionChoiceSetResponse(void)
{
}


DeleteInteractionChoiceSetResponse::DeleteInteractionChoiceSetResponse(void) : 
  RPC2Response(Marshaller::METHOD_DELETEINTERACTIONCHOICESETRESPONSE)
{
}


DeleteInteractionChoiceSetResponse::DeleteInteractionChoiceSetResponse(const DeleteInteractionChoiceSetResponse& c) : RPC2Response(Marshaller::METHOD_DELETEINTERACTIONCHOICESETRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteInteractionChoiceSetResponse::checkIntegrity(void)
{
  return DeleteInteractionChoiceSetResponseMarshaller::checkIntegrity(*this);
}
