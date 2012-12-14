#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/OnCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


OnCommand& OnCommand::operator =(const OnCommand& c)
{
  cmdID=c.cmdID;
  appId=c.appId;
  return *this;
}


OnCommand::~OnCommand(void)
{
}


OnCommand::OnCommand(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND)
{
}


OnCommand::OnCommand(const OnCommand& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND)
{
  *this=c;
}


unsigned int OnCommand::get_cmdID(void)
{
  return cmdID;
}

bool OnCommand::set_cmdID(unsigned int cmdID_)
{
  cmdID=cmdID_;
  return true;
}

int OnCommand::get_appId(void)
{
  return appId;
}

bool OnCommand::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool OnCommand::checkIntegrity(void)
{
  return OnCommandMarshaller::checkIntegrity(*this);
}
