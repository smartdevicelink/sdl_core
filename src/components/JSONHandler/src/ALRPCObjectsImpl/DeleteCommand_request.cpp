#include "../../include/JSONHandler/ALRPCObjects/DeleteCommand_request.h"
#include "DeleteCommand_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

