#ifndef NSAPPLINKRPC_VEHICLEDATARESULTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_VEHICLEDATARESULTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/VehicleDataResult.h"


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
