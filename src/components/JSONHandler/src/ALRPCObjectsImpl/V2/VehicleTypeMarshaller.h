#ifndef NSAPPLINKRPCV2_VEHICLETYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_VEHICLETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct VehicleTypeMarshaller
  {
    static bool checkIntegrity(VehicleType& e);
    static bool checkIntegrityConst(const VehicleType& e);
  
    static bool fromString(const std::string& s,VehicleType& e);
    static const std::string toString(const VehicleType& e);
  
    static bool fromJSON(const Json::Value& s,VehicleType& e);
    static Json::Value toJSON(const VehicleType& e);
  };
}

#endif
