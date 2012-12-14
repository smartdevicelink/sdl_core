#ifndef NSAPPLINKRPCV2_MENUPARAMSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_MENUPARAMSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/MenuParams.h"


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

  struct MenuParamsMarshaller
  {
    static bool checkIntegrity(MenuParams& e);
    static bool checkIntegrityConst(const MenuParams& e);
  
    static bool fromString(const std::string& s,MenuParams& e);
    static const std::string toString(const MenuParams& e);
  
    static bool fromJSON(const Json::Value& s,MenuParams& e);
    static Json::Value toJSON(const MenuParams& e);
  };
}

#endif
