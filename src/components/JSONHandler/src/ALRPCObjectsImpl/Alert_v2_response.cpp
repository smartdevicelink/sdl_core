#include "../include/JSONHandler/ALRPCObjects/Alert_v2_response.h"
#include "Alert_v2_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "Result_v2Marshaller.h"

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
Alert_v2_response& Alert_v2_response::operator =(const Alert_v2_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
  tryAgainTime= c.tryAgainTime;

  return *this;}


Alert_v2_response::~Alert_v2_response(void)
{
  if(info)
    delete info;
}


Alert_v2_response::Alert_v2_response(const Alert_v2_response& c)
{
  *this=c;
}


bool Alert_v2_response::checkIntegrity(void)
{
  return Alert_v2_responseMarshaller::checkIntegrity(*this);
}


Alert_v2_response::Alert_v2_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_ALERT_V2_RESPONSE),
      info(0)
{
}



bool Alert_v2_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool Alert_v2_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool Alert_v2_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void Alert_v2_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool Alert_v2_response::set_tryAgainTime(unsigned int tryAgainTime_)
{
  if(tryAgainTime_>2000000000)  return false;
  tryAgainTime=tryAgainTime_;
  return true;
}




bool Alert_v2_response::get_success(void) const
{
  return success;
}

const Result_v2& Alert_v2_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* Alert_v2_response::get_info(void) const 
{
  return info;
}

unsigned int Alert_v2_response::get_tryAgainTime(void) const
{
  return tryAgainTime;
}

