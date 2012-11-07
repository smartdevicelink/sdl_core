#include "../include/JSONHandler/RPC2Objects/VR/DeleteCommand.h"
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


DeleteCommand& DeleteCommand::operator =(const DeleteCommand& c)
{
  cmdId=c.cmdId;
  return *this;
}


DeleteCommand::~DeleteCommand(void)
{
}


DeleteCommand::DeleteCommand(void) : 
  RPC2Request(Marshaller::METHOD_DELETECOMMAND)
{
}


DeleteCommand::DeleteCommand(const DeleteCommand& c) : RPC2Request(Marshaller::METHOD_DELETECOMMAND,c.getId())
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
