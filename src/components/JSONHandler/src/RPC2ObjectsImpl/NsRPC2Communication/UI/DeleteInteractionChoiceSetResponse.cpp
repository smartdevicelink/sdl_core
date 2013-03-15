//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
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
