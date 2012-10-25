#include "../../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_request.h"
#include "DeleteInteractionChoiceSet_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


DeleteInteractionChoiceSet_request::~DeleteInteractionChoiceSet_request(void)
{
}


DeleteInteractionChoiceSet_request::DeleteInteractionChoiceSet_request(const DeleteInteractionChoiceSet_request& c)
{
  *this=c;
}


bool DeleteInteractionChoiceSet_request::checkIntegrity(void)
{
  return DeleteInteractionChoiceSet_requestMarshaller::checkIntegrity(*this);
}


DeleteInteractionChoiceSet_request::DeleteInteractionChoiceSet_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DELETEINTERACTIONCHOICESET_REQUEST)
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

