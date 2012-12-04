#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnCommand& OnCommand::operator =(const OnCommand& c)
{
  commandId=c.commandId;
  appId=c.appId;
  return *this;
}


OnCommand::~OnCommand(void)
{
}


OnCommand::OnCommand(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND)
{
}


OnCommand::OnCommand(const OnCommand& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND)
{
  *this=c;
}


unsigned int OnCommand::get_commandId(void)
{
  return commandId;
}

bool OnCommand::set_commandId(unsigned int commandId_)
{
  commandId=commandId_;
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
