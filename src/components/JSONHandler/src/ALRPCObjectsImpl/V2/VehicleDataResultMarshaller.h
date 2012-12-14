#ifndef NSAPPLINKRPCV2_VEHICLEDATARESULTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_VEHICLEDATARESULTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleDataResult.h"


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

  struct VehicleDataResultMarshaller
  {
    static bool checkIntegrity(VehicleDataResult& e);
    static bool checkIntegrityConst(const VehicleDataResult& e);
  
    static bool fromString(const std::string& s,VehicleDataResult& e);
    static const std::string toString(const VehicleDataResult& e);
  
    static bool fromJSON(const Json::Value& s,VehicleDataResult& e);
    static Json::Value toJSON(const VehicleDataResult& e);
  };
}

#endif
