#include "../../include/JSONHandler/ALRPCObjects/SubscribeVehicleData_response.h"
#include "SubscribeVehicleData_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"
#include "VehicleDataResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

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

bool SubscribeVehicleData_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
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

const Result& SubscribeVehicleData_response::get_resultCode(void) const 
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

