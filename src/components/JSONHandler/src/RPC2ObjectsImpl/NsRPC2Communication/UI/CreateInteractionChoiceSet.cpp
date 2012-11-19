#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/CreateInteractionChoiceSet.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


CreateInteractionChoiceSet& CreateInteractionChoiceSet::operator =(const CreateInteractionChoiceSet& c)
{
  interactionChoiceSetID=c.interactionChoiceSetID;
  choiceSet=c.choiceSet;
  return *this;
}


CreateInteractionChoiceSet::~CreateInteractionChoiceSet(void)
{
}


CreateInteractionChoiceSet::CreateInteractionChoiceSet(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET)
{
}


CreateInteractionChoiceSet::CreateInteractionChoiceSet(const CreateInteractionChoiceSet& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET,c.getId())
{
  *this=c;
}


unsigned int CreateInteractionChoiceSet::get_interactionChoiceSetID(void)
{
  return interactionChoiceSetID;
}

bool CreateInteractionChoiceSet::set_interactionChoiceSetID(unsigned int interactionChoiceSetID_)
{
  interactionChoiceSetID=interactionChoiceSetID_;
  return true;
}

const std::vector< NsAppLinkRPC::Choice>& CreateInteractionChoiceSet::get_choiceSet(void)
{
  return choiceSet;
}

bool CreateInteractionChoiceSet::set_choiceSet(const std::vector< NsAppLinkRPC::Choice>& choiceSet_)
{
  choiceSet=choiceSet_;
  return true;
}

bool CreateInteractionChoiceSet::checkIntegrity(void)
{
  return CreateInteractionChoiceSetMarshaller::checkIntegrity(*this);
}
