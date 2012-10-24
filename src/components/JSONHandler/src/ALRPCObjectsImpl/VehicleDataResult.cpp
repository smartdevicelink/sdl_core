#include "../../include/JSONHandler/ALRPCObjects/VehicleDataResult.h"
#include "VehicleDataResultMarshaller.h"
#include "VehicleDataResultCodeMarshaller.h"
#include "VehicleDataTypeMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



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


