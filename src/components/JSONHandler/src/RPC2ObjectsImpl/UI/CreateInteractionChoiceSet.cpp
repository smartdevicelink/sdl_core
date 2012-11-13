#include "../include/JSONHandler/RPC2Objects/UI/CreateInteractionChoiceSet.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


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
  RPC2Request(Marshaller::METHOD_CREATEINTERACTIONCHOICESET)
{
}


CreateInteractionChoiceSet::CreateInteractionChoiceSet(const CreateInteractionChoiceSet& c) : RPC2Request(Marshaller::METHOD_CREATEINTERACTIONCHOICESET,c.getId())
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

const std::vector< Choice>& CreateInteractionChoiceSet::get_choiceSet(void)
{
  return choiceSet;
}

bool CreateInteractionChoiceSet::set_choiceSet(const std::vector< Choice>& choiceSet_)
{
  choiceSet=choiceSet_;
  return true;
}

bool CreateInteractionChoiceSet::checkIntegrity(void)
{
  return CreateInteractionChoiceSetMarshaller::checkIntegrity(*this);
}
