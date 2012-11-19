#include "../include/JSONHandler/ALRPCObjects/AddSubMenu_response.h"
#include "AddSubMenu_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 06:40:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
AddSubMenu_response& AddSubMenu_response::operator =(const AddSubMenu_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


AddSubMenu_response::~AddSubMenu_response(void)
{
  if(info)
    delete info;
}


AddSubMenu_response::AddSubMenu_response(const AddSubMenu_response& c)
{
  *this=c;
}


bool AddSubMenu_response::checkIntegrity(void)
{
  return AddSubMenu_responseMarshaller::checkIntegrity(*this);
}


AddSubMenu_response::AddSubMenu_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_ADDSUBMENU_RESPONSE),
      info(0)
{
}



bool AddSubMenu_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool AddSubMenu_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool AddSubMenu_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void AddSubMenu_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool AddSubMenu_response::get_success(void) const
{
  return success;
}

const Result& AddSubMenu_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* AddSubMenu_response::get_info(void) const 
{
  return info;
}

