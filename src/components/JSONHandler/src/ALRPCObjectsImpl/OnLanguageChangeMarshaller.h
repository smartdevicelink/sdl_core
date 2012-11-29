#ifndef NSAPPLINKRPC_ONLANGUAGECHANGEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONLANGUAGECHANGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnLanguageChange.h"


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
