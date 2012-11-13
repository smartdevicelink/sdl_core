#ifndef APPLINKRPC_ONBUTTONPRESSMARSHALLER_INCLUDE
#define APPLINKRPC_ONBUTTONPRESSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnButtonPress.h"


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

  struct OnButtonPressMarshaller
  {
    static bool checkIntegrity(OnButtonPress& e);
    static bool checkIntegrityConst(const OnButtonPress& e);
  
    static bool fromString(const std::string& s,OnButtonPress& e);
    static const std::string toString(const OnButtonPress& e);
  
    static bool fromJSON(const Json::Value& s,OnButtonPress& e);
    static Json::Value toJSON(const OnButtonPress& e);
  };
}

#endif
