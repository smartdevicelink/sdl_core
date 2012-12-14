#ifndef NSAPPLINKRPCV2_DTCMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DTCMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/DTC.h"


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

  struct DTCMarshaller
  {
    static bool checkIntegrity(DTC& e);
    static bool checkIntegrityConst(const DTC& e);
  
    static bool fromString(const std::string& s,DTC& e);
    static const std::string toString(const DTC& e);
  
    static bool fromJSON(const Json::Value& s,DTC& e);
    static Json::Value toJSON(const DTC& e);
  };
}

#endif
