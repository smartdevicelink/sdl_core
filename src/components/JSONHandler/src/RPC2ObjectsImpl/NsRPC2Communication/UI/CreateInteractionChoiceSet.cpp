//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/CreateInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


CreateInteractionChoiceSet& CreateInteractionChoiceSet::operator =(const CreateInteractionChoiceSet& c)
{
  interactionChoiceSetID=c.interactionChoiceSetID;
  choiceSet=c.choiceSet;
  appId=c.appId;
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

const std::vector< NsSmartDeviceLinkRPCV2::Choice>& CreateInteractionChoiceSet::get_choiceSet(void)
{
  return choiceSet;
}

bool CreateInteractionChoiceSet::set_choiceSet(const std::vector< NsSmartDeviceLinkRPCV2::Choice>& choiceSet_)
{
  choiceSet=choiceSet_;
  return true;
}

int CreateInteractionChoiceSet::get_appId(void)
{
  return appId;
}

bool CreateInteractionChoiceSet::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool CreateInteractionChoiceSet::checkIntegrity(void)
{
  return CreateInteractionChoiceSetMarshaller::checkIntegrity(*this);
}
