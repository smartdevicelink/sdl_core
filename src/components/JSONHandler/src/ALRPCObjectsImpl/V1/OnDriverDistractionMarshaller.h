#ifndef NSAPPLINKRPC_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct OnDriverDistractionMarshaller
  {
    static bool checkIntegrity(OnDriverDistraction& e);
    static bool checkIntegrityConst(const OnDriverDistraction& e);
  
    static bool fromString(const std::string& s,OnDriverDistraction& e);
    static const std::string toString(const OnDriverDistraction& e);
  
    static bool fromJSON(const Json::Value& s,OnDriverDistraction& e);
    static Json::Value toJSON(const OnDriverDistraction& e);
  };
}

#endif
