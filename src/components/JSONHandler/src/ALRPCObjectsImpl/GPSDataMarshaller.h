#ifndef NSAPPLINKRPC_GPSDATAMARSHALLER_INCLUDE
#define NSAPPLINKRPC_GPSDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/GPSData.h"


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

  struct GPSDataMarshaller
  {
    static bool checkIntegrity(GPSData& e);
    static bool checkIntegrityConst(const GPSData& e);
  
    static bool fromString(const std::string& s,GPSData& e);
    static const std::string toString(const GPSData& e);
  
    static bool fromJSON(const Json::Value& s,GPSData& e);
    static Json::Value toJSON(const GPSData& e);
  };
}

#endif
