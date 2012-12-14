#ifndef NSAPPLINKRPC_MENUPARAMSMARSHALLER_INCLUDE
#define NSAPPLINKRPC_MENUPARAMSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/MenuParams.h"


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
