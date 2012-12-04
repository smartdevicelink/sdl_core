#ifndef NSAPPLINKRPCV2_ONLANGUAGECHANGEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONLANGUAGECHANGEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnLanguageChange.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct OnLanguageChangeMarshaller
  {
    static bool checkIntegrity(OnLanguageChange& e);
    static bool checkIntegrityConst(const OnLanguageChange& e);
  
    static bool fromString(const std::string& s,OnLanguageChange& e);
    static const std::string toString(const OnLanguageChange& e);
  
    static bool fromJSON(const Json::Value& s,OnLanguageChange& e);
    static Json::Value toJSON(const OnLanguageChange& e);
  };
}

#endif
