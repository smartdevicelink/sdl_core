#include "../include/JSONHandler/ALRPCObjects/V1/DeleteCommand_request.h"
#include "DeleteCommand_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

DeleteCommand_request::~DeleteCommand_request(void)
{
}


DeleteCommand_request::DeleteCommand_request(const DeleteCommand_request& c)
{
  *this=c;
}


bool DeleteCommand_request::checkIntegrity(void)
{
  return DeleteCommand_requestMarshaller::checkIntegrity(*this);
}


DeleteCommand_request::DeleteCommand_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DELETECOMMAND_REQUEST)
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

