#ifndef NSAPPLINKRPC_VEHICLETYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_VEHICLETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/VehicleType.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
