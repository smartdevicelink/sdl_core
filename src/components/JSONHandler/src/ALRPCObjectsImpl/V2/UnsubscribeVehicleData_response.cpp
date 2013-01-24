#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeVehicleData_response.h"
#include "UnsubscribeVehicleData_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"
#include "VehicleDataResultMarshaller.h"

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
UnsubscribeVehicleData_response& UnsubscribeVehicleData_response::operator =(const UnsubscribeVehicleData_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
  dataResult= c.dataResult ? new std::vector<VehicleDataResult>(c.dataResult[0]) : 0;

  return *this;
}


UnsubscribeVehicleData_response::~UnsubscribeVehicleData_response(void)
{
  if(info)
    delete info;
  if(dataResult)
    delete dataResult;
}


UnsubscribeVehicleData_response::UnsubscribeVehicleData_response(const UnsubscribeVehicleData_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool UnsubscribeVehicleData_response::checkIntegrity(void)
{
  return UnsubscribeVehicleData_responseMarshaller::checkIntegrity(*this);
}


UnsubscribeVehicleData_response::UnsubscribeVehicleData_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0),
    dataResult(0)
{
}



bool UnsubscribeVehicleData_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool UnsubscribeVehicleData_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool UnsubscribeVehicleData_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void UnsubscribeVehicleData_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool UnsubscribeVehicleData_response::set_dataResult(const std::vector<VehicleDataResult>& dataResult_)
{
  unsigned int i=dataResult_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!VehicleDataResultMarshaller::checkIntegrityConst(dataResult_[i]))   return false;
  }
  delete dataResult;
  dataResult=0;

  dataResult=new std::vector<VehicleDataResult>(dataResult_);
  return true;
}

void UnsubscribeVehicleData_response::reset_dataResult(void)
{
  if(dataResult)
    delete dataResult;
  dataResult=0;
}




bool UnsubscribeVehicleData_response::get_success(void) const
{
  return success;
}

const Result& UnsubscribeVehicleData_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* UnsubscribeVehicleData_response::get_info(void) const 
{
  return info;
}

const std::vector<VehicleDataResult>* UnsubscribeVehicleData_response::get_dataResult(void) const 
{
  return dataResult;
}

