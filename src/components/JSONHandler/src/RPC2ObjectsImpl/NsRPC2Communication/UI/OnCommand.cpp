#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnCommand.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnCommand& OnCommand::operator =(const OnCommand& c)
{
  commandId=c.commandId;
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

bool OnCommand::checkIntegrity(void)
{
  return OnCommandMarshaller::checkIntegrity(*this);
}
