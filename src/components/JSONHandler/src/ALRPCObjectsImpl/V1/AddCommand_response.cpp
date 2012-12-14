#include "../include/JSONHandler/ALRPCObjects/V1/AddCommand_response.h"
#include "AddCommand_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
AddCommand_response& AddCommand_response::operator =(const AddCommand_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


AddCommand_response::~AddCommand_response(void)
{
  if(info)
    delete info;
}


AddCommand_response::AddCommand_response(const AddCommand_response& c)
{
  *this=c;
}


bool AddCommand_response::checkIntegrity(void)
{
  return AddCommand_responseMarshaller::checkIntegrity(*this);
}


AddCommand_response::AddCommand_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_ADDCOMMAND_RESPONSE),
      info(0)
{
}



bool AddCommand_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool AddCommand_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool AddCommand_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void AddCommand_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool AddCommand_response::get_success(void) const
{
  return success;
}

const Result& AddCommand_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* AddCommand_response::get_info(void) const 
{
  return info;
}

