#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/UnsubscribeVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


UnsubscribeVehicleData& UnsubscribeVehicleData::operator =(const UnsubscribeVehicleData& c)
{
  dataType=c.dataType;
  appId=c.appId;
  return *this;
}


UnsubscribeVehicleData::~UnsubscribeVehicleData(void)
{
}


UnsubscribeVehicleData::UnsubscribeVehicleData(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATA)
{
}


UnsubscribeVehicleData::UnsubscribeVehicleData(const UnsubscribeVehicleData& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATA,c.getId())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::VehicleDataType>& UnsubscribeVehicleData::get_dataType(void)
{
  return dataType;
}

bool UnsubscribeVehicleData::set_dataType(const std::vector< NsAppLinkRPCV2::VehicleDataType>& dataType_)
{
  dataType=dataType_;
  return true;
}

int UnsubscribeVehicleData::get_appId(void)
{
  return appId;
}

bool UnsubscribeVehicleData::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool UnsubscribeVehicleData::checkIntegrity(void)
{
  return UnsubscribeVehicleDataMarshaller::checkIntegrity(*this);
}
