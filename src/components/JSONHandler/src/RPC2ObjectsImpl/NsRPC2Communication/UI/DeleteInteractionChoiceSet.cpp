#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


DeleteInteractionChoiceSet& DeleteInteractionChoiceSet::operator =(const DeleteInteractionChoiceSet& c)
{
  interactionChoiceSetID=c.interactionChoiceSetID;
  appId=c.appId;
  return *this;
}


DeleteInteractionChoiceSet::~DeleteInteractionChoiceSet(void)
{
}


DeleteInteractionChoiceSet::DeleteInteractionChoiceSet(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET)
{
}


DeleteInteractionChoiceSet::DeleteInteractionChoiceSet(const DeleteInteractionChoiceSet& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET,c.getId())
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

int DeleteInteractionChoiceSet::get_appId(void)
{
  return appId;
}

bool DeleteInteractionChoiceSet::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool DeleteInteractionChoiceSet::checkIntegrity(void)
{
  return DeleteInteractionChoiceSetMarshaller::checkIntegrity(*this);
}
