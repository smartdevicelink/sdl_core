#include "../include/JSONHandler/ALRPCObjects/DeleteCommand_v2_request.h"
#include "DeleteCommand_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

DeleteCommand_v2_request::~DeleteCommand_v2_request(void)
{
}


DeleteCommand_v2_request::DeleteCommand_v2_request(const DeleteCommand_v2_request& c)
{
  *this=c;
}


bool DeleteCommand_v2_request::checkIntegrity(void)
{
  return DeleteCommand_v2_requestMarshaller::checkIntegrity(*this);
}


DeleteCommand_v2_request::DeleteCommand_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DELETECOMMAND_V2_REQUEST)
{
}



bool DeleteCommand_v2_request::set_cmdID(unsigned int cmdID_)
{
  if(cmdID_>2000000000)  return false;
  cmdID=cmdID_;
  return true;
}




unsigned int DeleteCommand_v2_request::get_cmdID(void) const
{
  return cmdID;
}

