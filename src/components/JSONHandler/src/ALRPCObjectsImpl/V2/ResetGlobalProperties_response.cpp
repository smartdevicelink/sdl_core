#include "../include/JSONHandler/ALRPCObjects/V2/ResetGlobalProperties_response.h"
#include "ResetGlobalProperties_responseMarshaller.h"
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
ResetGlobalProperties_response& ResetGlobalProperties_response::operator =(const ResetGlobalProperties_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


ResetGlobalProperties_response::~ResetGlobalProperties_response(void)
{
  if(info)
    delete info;
}


ResetGlobalProperties_response::ResetGlobalProperties_response(const ResetGlobalProperties_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool ResetGlobalProperties_response::checkIntegrity(void)
{
  return ResetGlobalProperties_responseMarshaller::checkIntegrity(*this);
}


ResetGlobalProperties_response::ResetGlobalProperties_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0)
{
}



bool ResetGlobalProperties_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool ResetGlobalProperties_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool ResetGlobalProperties_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void ResetGlobalProperties_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool ResetGlobalProperties_response::get_success(void) const
{
  return success;
}

const Result& ResetGlobalProperties_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* ResetGlobalProperties_response::get_info(void) const 
{
  return info;
}

