#include "../include/JSONHandler/ALRPCObjects/V1/Alert_response.h"
#include "Alert_responseMarshaller.h"
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
Alert_response& Alert_response::operator =(const Alert_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


Alert_response::~Alert_response(void)
{
  if(info)
    delete info;
}


Alert_response::Alert_response(const Alert_response& c)
{
  *this=c;
}


bool Alert_response::checkIntegrity(void)
{
  return Alert_responseMarshaller::checkIntegrity(*this);
}


Alert_response::Alert_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_ALERT_RESPONSE),
      info(0)
{
}



bool Alert_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool Alert_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool Alert_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void Alert_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool Alert_response::get_success(void) const
{
  return success;
}

const Result& Alert_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* Alert_response::get_info(void) const 
{
  return info;
}

