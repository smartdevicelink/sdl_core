#include "../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_v2_request.h"
#include "CreateInteractionChoiceSet_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ChoiceMarshaller.h"

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

CreateInteractionChoiceSet_v2_request::~CreateInteractionChoiceSet_v2_request(void)
{
}


CreateInteractionChoiceSet_v2_request::CreateInteractionChoiceSet_v2_request(const CreateInteractionChoiceSet_v2_request& c)
{
  *this=c;
}


bool CreateInteractionChoiceSet_v2_request::checkIntegrity(void)
{
  return CreateInteractionChoiceSet_v2_requestMarshaller::checkIntegrity(*this);
}


CreateInteractionChoiceSet_v2_request::CreateInteractionChoiceSet_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_CREATEINTERACTIONCHOICESET_V2_REQUEST)
{
}



bool CreateInteractionChoiceSet_v2_request::set_interactionChoiceSetID(unsigned int interactionChoiceSetID_)
{
  if(interactionChoiceSetID_>2000000000)  return false;
  interactionChoiceSetID=interactionChoiceSetID_;
  return true;
}

bool CreateInteractionChoiceSet_v2_request::set_choiceSet(const std::vector<Choice>& choiceSet_)
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




unsigned int CreateInteractionChoiceSet_v2_request::get_interactionChoiceSetID(void) const
{
  return interactionChoiceSetID;
}

const std::vector<Choice>& CreateInteractionChoiceSet_v2_request::get_choiceSet(void) const 
{
  return choiceSet;
}

