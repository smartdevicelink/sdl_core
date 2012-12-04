#ifndef NSAPPLINKRPCV2_SUBSCRIBEVEHICLEDATA_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SUBSCRIBEVEHICLEDATA_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeVehicleData_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct SubscribeVehicleData_requestMarshaller
  {
    static bool checkIntegrity(SubscribeVehicleData_request& e);
    static bool checkIntegrityConst(const SubscribeVehicleData_request& e);
  
    static bool fromString(const std::string& s,SubscribeVehicleData_request& e);
    static const std::string toString(const SubscribeVehicleData_request& e);
  
    static bool fromJSON(const Json::Value& s,SubscribeVehicleData_request& e);
    static Json::Value toJSON(const SubscribeVehicleData_request& e);
  };
}

#endif
