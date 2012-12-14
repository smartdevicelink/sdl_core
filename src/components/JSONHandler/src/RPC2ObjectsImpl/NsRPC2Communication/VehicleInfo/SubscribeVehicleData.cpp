#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/SubscribeVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


SubscribeVehicleData& SubscribeVehicleData::operator =(const SubscribeVehicleData& c)
{
  dataType=c.dataType;
  appId=c.appId;
  return *this;
}


SubscribeVehicleData::~SubscribeVehicleData(void)
{
}


SubscribeVehicleData::SubscribeVehicleData(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATA)
{
}


SubscribeVehicleData::SubscribeVehicleData(const SubscribeVehicleData& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATA,c.getId())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::VehicleDataType>& SubscribeVehicleData::get_dataType(void)
{
  return dataType;
}

bool SubscribeVehicleData::set_dataType(const std::vector< NsAppLinkRPCV2::VehicleDataType>& dataType_)
{
  dataType=dataType_;
  return true;
}

int SubscribeVehicleData::get_appId(void)
{
  return appId;
}

bool SubscribeVehicleData::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool SubscribeVehicleData::checkIntegrity(void)
{
  return SubscribeVehicleDataMarshaller::checkIntegrity(*this);
}
