#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


DeleteInteractionChoiceSetResponse& DeleteInteractionChoiceSetResponse::operator =(const DeleteInteractionChoiceSetResponse& c)
{
  return *this;
}


DeleteInteractionChoiceSetResponse::~DeleteInteractionChoiceSetResponse(void)
{
}


DeleteInteractionChoiceSetResponse::DeleteInteractionChoiceSetResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE)
{
}


DeleteInteractionChoiceSetResponse::DeleteInteractionChoiceSetResponse(const DeleteInteractionChoiceSetResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteInteractionChoiceSetResponse::checkIntegrity(void)
{
  return DeleteInteractionChoiceSetResponseMarshaller::checkIntegrity(*this);
}
