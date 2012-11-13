#include "../include/JSONHandler/RPC2Objects/UI/CreateInteractionChoiceSetResponse.h"
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


CreateInteractionChoiceSetResponse& CreateInteractionChoiceSetResponse::operator =(const CreateInteractionChoiceSetResponse& c)
{
  return *this;
}


CreateInteractionChoiceSetResponse::~CreateInteractionChoiceSetResponse(void)
{
}


CreateInteractionChoiceSetResponse::CreateInteractionChoiceSetResponse(void) : 
  RPC2Response(Marshaller::METHOD_CREATEINTERACTIONCHOICESETRESPONSE)
{
}


CreateInteractionChoiceSetResponse::CreateInteractionChoiceSetResponse(const CreateInteractionChoiceSetResponse& c) : RPC2Response(Marshaller::METHOD_CREATEINTERACTIONCHOICESETRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool CreateInteractionChoiceSetResponse::checkIntegrity(void)
{
  return CreateInteractionChoiceSetResponseMarshaller::checkIntegrity(*this);
}
