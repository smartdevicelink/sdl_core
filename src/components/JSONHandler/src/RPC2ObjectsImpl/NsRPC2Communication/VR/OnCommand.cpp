#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/OnCommand.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


OnCommand& OnCommand::operator =(const OnCommand& c)
{
  cmdID=c.cmdID;
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

bool OnCommand::checkIntegrity(void)
{
  return OnCommandMarshaller::checkIntegrity(*this);
}
