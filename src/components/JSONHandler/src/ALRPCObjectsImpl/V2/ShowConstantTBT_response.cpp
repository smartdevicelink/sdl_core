#include "../include/JSONHandler/ALRPCObjects/V2/ShowConstantTBT_response.h"
#include "ShowConstantTBT_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
ShowConstantTBT_response& ShowConstantTBT_response::operator =(const ShowConstantTBT_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


ShowConstantTBT_response::~ShowConstantTBT_response(void)
{
  if(info)
    delete info;
}


ShowConstantTBT_response::ShowConstantTBT_response(const ShowConstantTBT_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool ShowConstantTBT_response::checkIntegrity(void)
{
  return ShowConstantTBT_responseMarshaller::checkIntegrity(*this);
}


ShowConstantTBT_response::ShowConstantTBT_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0)
{
}



bool ShowConstantTBT_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool ShowConstantTBT_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool ShowConstantTBT_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void ShowConstantTBT_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool ShowConstantTBT_response::get_success(void) const
{
  return success;
}

const Result& ShowConstantTBT_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* ShowConstantTBT_response::get_info(void) const 
{
  return info;
}

