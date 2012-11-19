#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


DeleteCommand& DeleteCommand::operator =(const DeleteCommand& c)
{
  cmdId=c.cmdId;
  return *this;
}


DeleteCommand::~DeleteCommand(void)
{
}


DeleteCommand::DeleteCommand(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND)
{
}


DeleteCommand::DeleteCommand(const DeleteCommand& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND,c.getId())
{
  *this=c;
}


unsigned int DeleteCommand::get_cmdId(void)
{
  return cmdId;
}

bool DeleteCommand::set_cmdId(unsigned int cmdId_)
{
  cmdId=cmdId_;
  return true;
}

bool DeleteCommand::checkIntegrity(void)
{
  return DeleteCommandMarshaller::checkIntegrity(*this);
}
