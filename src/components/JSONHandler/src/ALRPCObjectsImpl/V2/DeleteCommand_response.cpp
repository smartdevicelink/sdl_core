#include "../include/JSONHandler/ALRPCObjects/V2/DeleteCommand_response.h"
#include "DeleteCommand_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
DeleteCommand_response& DeleteCommand_response::operator =(const DeleteCommand_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


DeleteCommand_response::~DeleteCommand_response(void)
{
  if(info)
    delete info;
}


DeleteCommand_response::DeleteCommand_response(const DeleteCommand_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool DeleteCommand_response::checkIntegrity(void)
{
  return DeleteCommand_responseMarshaller::checkIntegrity(*this);
}


DeleteCommand_response::DeleteCommand_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0)
{
}



bool DeleteCommand_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool DeleteCommand_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool DeleteCommand_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void DeleteCommand_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool DeleteCommand_response::get_success(void) const
{
  return success;
}

const Result& DeleteCommand_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* DeleteCommand_response::get_info(void) const 
{
  return info;
}

