#ifndef NSAPPLINKRPC_HMIAPPLICATIONMARSHALLER_INCLUDE
#define NSAPPLINKRPC_HMIAPPLICATIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/HMIApplication.h"


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

  struct HMIApplicationMarshaller
  {
    static bool checkIntegrity(HMIApplication& e);
    static bool checkIntegrityConst(const HMIApplication& e);
  
    static bool fromString(const std::string& s,HMIApplication& e);
    static const std::string toString(const HMIApplication& e);
  
    static bool fromJSON(const Json::Value& s,HMIApplication& e);
    static Json::Value toJSON(const HMIApplication& e);
  };
}

#endif
