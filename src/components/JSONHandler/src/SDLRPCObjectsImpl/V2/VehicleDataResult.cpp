//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/VehicleDataResult.h"
#include "VehicleDataResultMarshaller.h"
#include "VehicleDataResultCodeMarshaller.h"
#include "VehicleDataTypeMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

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


