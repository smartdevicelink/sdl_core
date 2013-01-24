#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


GetVehicleData& GetVehicleData::operator =(const GetVehicleData& c)
{
  dataType=c.dataType;
  appId=c.appId;
  return *this;
}


GetVehicleData::~GetVehicleData(void)
{
}


GetVehicleData::GetVehicleData(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATA)
{
}


GetVehicleData::GetVehicleData(const GetVehicleData& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATA,c.getId())
{
  *this=c;
}


const NsAppLinkRPCV2::VehicleDataType& GetVehicleData::get_dataType(void)
{
  return dataType;
}

bool GetVehicleData::set_dataType(const NsAppLinkRPCV2::VehicleDataType& dataType_)
{
  dataType=dataType_;
  return true;
}

int GetVehicleData::get_appId(void)
{
  return appId;
}

bool GetVehicleData::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool GetVehicleData::checkIntegrity(void)
{
  return GetVehicleDataMarshaller::checkIntegrity(*this);
}
