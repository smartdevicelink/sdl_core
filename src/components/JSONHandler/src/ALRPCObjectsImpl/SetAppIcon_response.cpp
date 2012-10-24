#include "../../include/JSONHandler/ALRPCObjects/SetAppIcon_response.h"
#include "SetAppIcon_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

SetAppIcon_response& SetAppIcon_response::operator =(const SetAppIcon_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


SetAppIcon_response::~SetAppIcon_response(void)
{
  if(info)
    delete info;
}


SetAppIcon_response::SetAppIcon_response(const SetAppIcon_response& c)
{
  *this=c;
}


bool SetAppIcon_response::checkIntegrity(void)
{
  return SetAppIcon_responseMarshaller::checkIntegrity(*this);
}


SetAppIcon_response::SetAppIcon_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_SETAPPICON_RESPONSE),
      info(0)
{
}



bool SetAppIcon_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool SetAppIcon_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool SetAppIcon_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void SetAppIcon_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool SetAppIcon_response::get_success(void) const
{
  return success;
}

const Result& SetAppIcon_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* SetAppIcon_response::get_info(void) const 
{
  return info;
}

