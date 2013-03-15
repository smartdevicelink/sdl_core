//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VR;


AddCommand& AddCommand::operator =(const AddCommand& c)
{
  cmdId=c.cmdId;
  vrCommands=c.vrCommands;
  appId=c.appId;
  return *this;
}


AddCommand::~AddCommand(void)
{
}


AddCommand::AddCommand(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND)
{
}


AddCommand::AddCommand(const AddCommand& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND,c.getId())
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

int AddCommand::get_appId(void)
{
  return appId;
}

bool AddCommand::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool AddCommand::checkIntegrity(void)
{
  return AddCommandMarshaller::checkIntegrity(*this);
}
