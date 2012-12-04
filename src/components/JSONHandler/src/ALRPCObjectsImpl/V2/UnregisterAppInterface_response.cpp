#include "../include/JSONHandler/ALRPCObjects/V2/UnregisterAppInterface_response.h"
#include "UnregisterAppInterface_responseMarshaller.h"
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
UnregisterAppInterface_response& UnregisterAppInterface_response::operator =(const UnregisterAppInterface_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


UnregisterAppInterface_response::~UnregisterAppInterface_response(void)
{
  if(info)
    delete info;
}


UnregisterAppInterface_response::UnregisterAppInterface_response(const UnregisterAppInterface_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool UnregisterAppInterface_response::checkIntegrity(void)
{
  return UnregisterAppInterface_responseMarshaller::checkIntegrity(*this);
}


UnregisterAppInterface_response::UnregisterAppInterface_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0)
{
}



bool UnregisterAppInterface_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool UnregisterAppInterface_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool UnregisterAppInterface_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void UnregisterAppInterface_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool UnregisterAppInterface_response::get_success(void) const
{
  return success;
}

const Result& UnregisterAppInterface_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* UnregisterAppInterface_response::get_info(void) const 
{
  return info;
}

