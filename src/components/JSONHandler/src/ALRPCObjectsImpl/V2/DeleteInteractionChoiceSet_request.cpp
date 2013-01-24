#include "../include/JSONHandler/ALRPCObjects/V2/DeleteInteractionChoiceSet_request.h"
#include "DeleteInteractionChoiceSet_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

DeleteInteractionChoiceSet_request::~DeleteInteractionChoiceSet_request(void)
{
}


DeleteInteractionChoiceSet_request::DeleteInteractionChoiceSet_request(const DeleteInteractionChoiceSet_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool DeleteInteractionChoiceSet_request::checkIntegrity(void)
{
  return DeleteInteractionChoiceSet_requestMarshaller::checkIntegrity(*this);
}


DeleteInteractionChoiceSet_request::DeleteInteractionChoiceSet_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool DeleteInteractionChoiceSet_request::set_interactionChoiceSetID(unsigned int interactionChoiceSetID_)
{
  if(interactionChoiceSetID_>2000000000)  return false;
  interactionChoiceSetID=interactionChoiceSetID_;
  return true;
}




unsigned int DeleteInteractionChoiceSet_request::get_interactionChoiceSetID(void) const
{
  return interactionChoiceSetID;
}

