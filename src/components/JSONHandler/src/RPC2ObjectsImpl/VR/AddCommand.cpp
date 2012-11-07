#include "../include/JSONHandler/RPC2Objects/VR/AddCommand.h"
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


AddCommand& AddCommand::operator =(const AddCommand& c)
{
  cmdId=c.cmdId;
  vrCommands=c.vrCommands;
  return *this;
}


AddCommand::~AddCommand(void)
{
}


AddCommand::AddCommand(void) : 
  RPC2Request(Marshaller::METHOD_ADDCOMMAND)
{
}


AddCommand::AddCommand(const AddCommand& c) : RPC2Request(Marshaller::METHOD_ADDCOMMAND,c.getId())
{
  *this=c;
}


unsigned int AddCommand::get_cmdId(void)
{
  return cmdId;
}

bool AddCommand::set_cmdId(unsigned int cmdId_)
{
  cmdId=cmdId_;
  return true;
}

const std::vector< std::string>& AddCommand::get_vrCommands(void)
{
  return vrCommands;
}

bool AddCommand::set_vrCommands(const std::vector< std::string>& vrCommands_)
{
  vrCommands=vrCommands_;
  return true;
}

bool AddCommand::checkIntegrity(void)
{
  return AddCommandMarshaller::checkIntegrity(*this);
}
