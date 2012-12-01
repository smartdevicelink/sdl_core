#include "../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_v2_request.h"
#include "DeleteInteractionChoiceSet_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

DeleteInteractionChoiceSet_v2_request::~DeleteInteractionChoiceSet_v2_request(void)
{
}


DeleteInteractionChoiceSet_v2_request::DeleteInteractionChoiceSet_v2_request(const DeleteInteractionChoiceSet_v2_request& c)
{
  *this=c;
}


bool DeleteInteractionChoiceSet_v2_request::checkIntegrity(void)
{
  return DeleteInteractionChoiceSet_v2_requestMarshaller::checkIntegrity(*this);
}


DeleteInteractionChoiceSet_v2_request::DeleteInteractionChoiceSet_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DELETEINTERACTIONCHOICESET_V2_REQUEST)
{
}



bool DeleteInteractionChoiceSet_v2_request::set_interactionChoiceSetID(unsigned int interactionChoiceSetID_)
{
  if(interactionChoiceSetID_>2000000000)  return false;
  interactionChoiceSetID=interactionChoiceSetID_;
  return true;
}




unsigned int DeleteInteractionChoiceSet_v2_request::get_interactionChoiceSetID(void) const
{
  return interactionChoiceSetID;
}

