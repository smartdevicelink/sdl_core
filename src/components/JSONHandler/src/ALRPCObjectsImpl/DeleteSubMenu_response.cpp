#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_response.h"
#include "DeleteSubMenu_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/

DeleteSubMenu_response& DeleteSubMenu_response::operator =(const DeleteSubMenu_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


DeleteSubMenu_response::~DeleteSubMenu_response(void)
{
  if(info)
    delete info;
}


DeleteSubMenu_response::DeleteSubMenu_response(const DeleteSubMenu_response& c)
{
  *this=c;
}


bool DeleteSubMenu_response::checkIntegrity(void)
{
  return DeleteSubMenu_responseMarshaller::checkIntegrity(*this);
}


DeleteSubMenu_response::DeleteSubMenu_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_DELETESUBMENU_RESPONSE),
      info(0)
{
}



bool DeleteSubMenu_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool DeleteSubMenu_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool DeleteSubMenu_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void DeleteSubMenu_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool DeleteSubMenu_response::get_success(void) const
{
  return success;
}

const Result& DeleteSubMenu_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* DeleteSubMenu_response::get_info(void) const 
{
  return info;
}

