#ifndef APPLINKRPC_MENUPARAMSMARSHALLER_INCLUDE
#define APPLINKRPC_MENUPARAMSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/MenuParams.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
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
