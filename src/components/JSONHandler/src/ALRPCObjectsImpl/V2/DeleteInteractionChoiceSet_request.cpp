#include "../include/JSONHandler/ALRPCObjects/V2/DeleteInteractionChoiceSet_request.h"
#include "DeleteInteractionChoiceSet_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

DeleteInteractionChoiceSet_request::~DeleteInteractionChoiceSet_request(void)
{
}


DeleteInteractionChoiceSet_request::DeleteInteractionChoiceSet_request(const DeleteInteractionChoiceSet_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool DeleteInteractionChoiceSet_request::checkIntegrity(void)
{
  return DeleteInteractionChoiceSet_requestMarshaller::checkIntegrity(*this);
}


DeleteInteractionChoiceSet_request::DeleteInteractionChoiceSet_request(void) : ALRPC2Message(PROTOCOL_VERSION)
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

