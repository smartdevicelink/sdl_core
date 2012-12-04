#ifndef NSAPPLINKRPCV1_HMIAPPLICATIONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_HMIAPPLICATIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/HMIApplication.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
