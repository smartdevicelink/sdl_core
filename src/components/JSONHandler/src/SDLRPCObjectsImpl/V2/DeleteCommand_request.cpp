#include "../include/JSONHandler/SDLRPCObjects/V2/DeleteCommand_request.h"
#include "DeleteCommand_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;

DeleteCommand_request::~DeleteCommand_request(void)
{
}


DeleteCommand_request::DeleteCommand_request(const DeleteCommand_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool DeleteCommand_request::checkIntegrity(void)
{
  return DeleteCommand_requestMarshaller::checkIntegrity(*this);
}


DeleteCommand_request::DeleteCommand_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool DeleteCommand_request::set_cmdID(unsigned int cmdID_)
{
  if(cmdID_>2000000000)  return false;
  cmdID=cmdID_;
  return true;
}




unsigned int DeleteCommand_request::get_cmdID(void) const
{
  return cmdID;
}

