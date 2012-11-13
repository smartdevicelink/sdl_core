#include "../include/JSONHandler/RPC2Objects/UI/DeleteInteractionChoiceSet.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::UI;


DeleteInteractionChoiceSet& DeleteInteractionChoiceSet::operator =(const DeleteInteractionChoiceSet& c)
{
  interactionChoiceSetID=c.interactionChoiceSetID;
  return *this;
}


DeleteInteractionChoiceSet::~DeleteInteractionChoiceSet(void)
{
}


DeleteInteractionChoiceSet::DeleteInteractionChoiceSet(void) : 
  RPC2Request(Marshaller::METHOD_DELETEINTERACTIONCHOICESET)
{
}


DeleteInteractionChoiceSet::DeleteInteractionChoiceSet(const DeleteInteractionChoiceSet& c) : RPC2Request(Marshaller::METHOD_DELETEINTERACTIONCHOICESET,c.getId())
{
  *this=c;
}


unsigned int DeleteInteractionChoiceSet::get_interactionChoiceSetID(void)
{
  return interactionChoiceSetID;
}

bool DeleteInteractionChoiceSet::set_interactionChoiceSetID(unsigned int interactionChoiceSetID_)
{
  interactionChoiceSetID=interactionChoiceSetID_;
  return true;
}

bool DeleteInteractionChoiceSet::checkIntegrity(void)
{
  return DeleteInteractionChoiceSetMarshaller::checkIntegrity(*this);
}
