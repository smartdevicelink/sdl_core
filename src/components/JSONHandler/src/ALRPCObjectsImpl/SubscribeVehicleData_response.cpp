#include "../include/JSONHandler/ALRPCObjects/SubscribeVehicleData_response.h"
#include "SubscribeVehicleData_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "Result_v2Marshaller.h"
#include "VehicleDataResultMarshaller.h"

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
SubscribeVehicleData_response& SubscribeVehicleData_response::operator =(const SubscribeVehicleData_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
  dataResult= c.dataResult ? new std::vector<VehicleDataResult>(c.dataResult[0]) : 0;

  return *this;}


SubscribeVehicleData_response::~SubscribeVehicleData_response(void)
{
  if(info)
    delete info;
  if(dataResult)
    delete dataResult;
}


SubscribeVehicleData_response::SubscribeVehicleData_response(const SubscribeVehicleData_response& c)
{
  *this=c;
}


bool SubscribeVehicleData_response::checkIntegrity(void)
{
  return SubscribeVehicleData_responseMarshaller::checkIntegrity(*this);
}


SubscribeVehicleData_response::SubscribeVehicleData_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_SUBSCRIBEVEHICLEDATA_RESPONSE),
      info(0),
    dataResult(0)
{
}



bool SubscribeVehicleData_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool SubscribeVehicleData_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool SubscribeVehicleData_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void SubscribeVehicleData_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool SubscribeVehicleData_response::set_dataResult(const std::vector<VehicleDataResult>& dataResult_)
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

void SubscribeVehicleData_response::reset_dataResult(void)
{
  if(dataResult)
    delete dataResult;
  dataResult=0;
}




bool SubscribeVehicleData_response::get_success(void) const
{
  return success;
}

const Result_v2& SubscribeVehicleData_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* SubscribeVehicleData_response::get_info(void) const 
{
  return info;
}

const std::vector<VehicleDataResult>* SubscribeVehicleData_response::get_dataResult(void) const 
{
  return dataResult;
}

