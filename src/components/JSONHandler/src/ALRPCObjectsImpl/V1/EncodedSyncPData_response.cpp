#include "../include/JSONHandler/ALRPCObjects/V1/EncodedSyncPData_response.h"
#include "EncodedSyncPData_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
EncodedSyncPData_response& EncodedSyncPData_response::operator =(const EncodedSyncPData_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


EncodedSyncPData_response::~EncodedSyncPData_response(void)
{
  if(info)
    delete info;
}


EncodedSyncPData_response::EncodedSyncPData_response(const EncodedSyncPData_response& c)
{
  *this=c;
}


bool EncodedSyncPData_response::checkIntegrity(void)
{
  return EncodedSyncPData_responseMarshaller::checkIntegrity(*this);
}


EncodedSyncPData_response::EncodedSyncPData_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_ENCODEDSYNCPDATA_RESPONSE),
      info(0)
{
}



bool EncodedSyncPData_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool EncodedSyncPData_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool EncodedSyncPData_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void EncodedSyncPData_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool EncodedSyncPData_response::get_success(void) const
{
  return success;
}

const Result& EncodedSyncPData_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* EncodedSyncPData_response::get_info(void) const 
{
  return info;
}

