//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VR;


DeleteCommand& DeleteCommand::operator =(const DeleteCommand& c)
{
  cmdId=c.cmdId;
  appId=c.appId;
  return *this;
}


DeleteCommand::~DeleteCommand(void)
{
}


DeleteCommand::DeleteCommand(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND)
{
}


DeleteCommand::DeleteCommand(const DeleteCommand& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND,c.getId())
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

int DeleteCommand::get_appId(void)
{
  return appId;
}

bool DeleteCommand::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool DeleteCommand::checkIntegrity(void)
{
  return DeleteCommandMarshaller::checkIntegrity(*this);
}
