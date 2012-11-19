#ifndef NSAPPLINKRPC_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
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
