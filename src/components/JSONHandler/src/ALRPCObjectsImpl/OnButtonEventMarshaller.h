#ifndef NSAPPLINKRPC_ONBUTTONEVENTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"


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

  struct OnButtonEventMarshaller
  {
    static bool checkIntegrity(OnButtonEvent& e);
    static bool checkIntegrityConst(const OnButtonEvent& e);
  
    static bool fromString(const std::string& s,OnButtonEvent& e);
    static const std::string toString(const OnButtonEvent& e);
  
    static bool fromJSON(const Json::Value& s,OnButtonEvent& e);
    static Json::Value toJSON(const OnButtonEvent& e);
  };
}

#endif
