#include "../include/JSONHandler/ALRPCObjects/V2/SetGlobalProperties_response.h"
#include "SetGlobalProperties_responseMarshaller.h"
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
SetGlobalProperties_response& SetGlobalProperties_response::operator =(const SetGlobalProperties_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


SetGlobalProperties_response::~SetGlobalProperties_response(void)
{
  if(info)
    delete info;
}


SetGlobalProperties_response::SetGlobalProperties_response(const SetGlobalProperties_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool SetGlobalProperties_response::checkIntegrity(void)
{
  return SetGlobalProperties_responseMarshaller::checkIntegrity(*this);
}


SetGlobalProperties_response::SetGlobalProperties_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0)
{
}



bool SetGlobalProperties_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool SetGlobalProperties_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool SetGlobalProperties_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void SetGlobalProperties_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool SetGlobalProperties_response::get_success(void) const
{
  return success;
}

const Result& SetGlobalProperties_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* SetGlobalProperties_response::get_info(void) const 
{
  return info;
}

