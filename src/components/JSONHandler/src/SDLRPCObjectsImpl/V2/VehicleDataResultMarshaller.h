#ifndef NSAPPLINKRPCV2_VEHICLEDATARESULTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_VEHICLEDATARESULTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/VehicleDataResult.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
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
