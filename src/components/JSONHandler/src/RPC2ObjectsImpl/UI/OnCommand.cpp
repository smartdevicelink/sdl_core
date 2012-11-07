#include "../include/JSONHandler/RPC2Objects/UI/OnCommand.h"
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


OnCommand& OnCommand::operator =(const OnCommand& c)
{
  commandId=c.commandId;
  return *this;
}


OnCommand::~OnCommand(void)
{
}


OnCommand::OnCommand(void) : 
  RPC2Notification(Marshaller::METHOD_ONCOMMAND)
{
}


OnCommand::OnCommand(const OnCommand& c) : RPC2Notification(Marshaller::METHOD_ONCOMMAND)
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
