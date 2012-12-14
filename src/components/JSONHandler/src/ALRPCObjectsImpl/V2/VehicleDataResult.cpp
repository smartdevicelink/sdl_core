#include "../include/JSONHandler/ALRPCObjects/V2/VehicleDataResult.h"
#include "VehicleDataResultMarshaller.h"
#include "VehicleDataResultCodeMarshaller.h"
#include "VehicleDataTypeMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

VehicleDataResult::VehicleDataResult(const VehicleDataResult& c)
{
  *this=c;
}


bool VehicleDataResult::checkIntegrity(void)
{
  return VehicleDataResultMarshaller::checkIntegrity(*this);
}


VehicleDataResult::VehicleDataResult(void)
{
}



bool VehicleDataResult::set_dataType(const VehicleDataType& dataType_)
{
  if(!VehicleDataTypeMarshaller::checkIntegrityConst(dataType_))   return false;
  dataType=dataType_;
  return true;
}

bool VehicleDataResult::set_resultCode(const VehicleDataResultCode& resultCode_)
{
  if(!VehicleDataResultCodeMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}




const VehicleDataType& VehicleDataResult::get_dataType(void) const 
{
  return dataType;
}


const VehicleDataResultCode& VehicleDataResult::get_resultCode(void) const 
{
  return resultCode;
}


