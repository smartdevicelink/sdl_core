#include "../include/JSONHandler/SDLRPCObjects/V2/CreateInteractionChoiceSet_request.h"
#include "CreateInteractionChoiceSet_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "ChoiceMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;

CreateInteractionChoiceSet_request::~CreateInteractionChoiceSet_request(void)
{
}


CreateInteractionChoiceSet_request::CreateInteractionChoiceSet_request(const CreateInteractionChoiceSet_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool CreateInteractionChoiceSet_request::checkIntegrity(void)
{
  return CreateInteractionChoiceSet_requestMarshaller::checkIntegrity(*this);
}


CreateInteractionChoiceSet_request::CreateInteractionChoiceSet_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
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

