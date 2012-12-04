#include "../include/JSONHandler/ALRPCObjects/V2/CreateInteractionChoiceSet_request.h"
#include "CreateInteractionChoiceSet_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ChoiceMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

CreateInteractionChoiceSet_request::~CreateInteractionChoiceSet_request(void)
{
}


CreateInteractionChoiceSet_request::CreateInteractionChoiceSet_request(const CreateInteractionChoiceSet_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool CreateInteractionChoiceSet_request::checkIntegrity(void)
{
  return CreateInteractionChoiceSet_requestMarshaller::checkIntegrity(*this);
}


CreateInteractionChoiceSet_request::CreateInteractionChoiceSet_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool CreateInteractionChoiceSet_request::set_interactionChoiceSetID(unsigned int interactionChoiceSetID_)
{
  if(interactionChoiceSetID_>2000000000)  return false;
  interactionChoiceSetID=interactionChoiceSetID_;
  return true;
}

bool CreateInteractionChoiceSet_request::set_choiceSet(const std::vector<Choice>& choiceSet_)
{
  unsigned int i=choiceSet_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!ChoiceMarshaller::checkIntegrityConst(choiceSet_[i]))   return false;
  }
  choiceSet=choiceSet_;
  return true;
}




unsigned int CreateInteractionChoiceSet_request::get_interactionChoiceSetID(void) const
{
  return interactionChoiceSetID;
}

const std::vector<Choice>& CreateInteractionChoiceSet_request::get_choiceSet(void) const 
{
  return choiceSet;
}

