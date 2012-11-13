#include "../include/JSONHandler/RPC2Objects/VR/OnCommand.h"
#include "../include/JSONHandler/RPC2Objects/VR/Marshaller.h"

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::VR;


OnCommand& OnCommand::operator =(const OnCommand& c)
{
  cmdID=c.cmdID;
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


unsigned int OnCommand::get_cmdID(void)
{
  return cmdID;
}

bool OnCommand::set_cmdID(unsigned int cmdID_)
{
  cmdID=cmdID_;
  return true;
}

bool OnCommand::checkIntegrity(void)
{
  return OnCommandMarshaller::checkIntegrity(*this);
}
