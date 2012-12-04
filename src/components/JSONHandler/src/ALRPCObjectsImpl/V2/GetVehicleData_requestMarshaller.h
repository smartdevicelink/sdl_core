#ifndef NSAPPLINKRPCV2_GETVEHICLEDATA_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_GETVEHICLEDATA_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/GetVehicleData_request.h"


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

  struct GetVehicleData_requestMarshaller
  {
    static bool checkIntegrity(GetVehicleData_request& e);
    static bool checkIntegrityConst(const GetVehicleData_request& e);
  
    static bool fromString(const std::string& s,GetVehicleData_request& e);
    static const std::string toString(const GetVehicleData_request& e);
  
    static bool fromJSON(const Json::Value& s,GetVehicleData_request& e);
    static Json::Value toJSON(const GetVehicleData_request& e);
  };
}

#endif
