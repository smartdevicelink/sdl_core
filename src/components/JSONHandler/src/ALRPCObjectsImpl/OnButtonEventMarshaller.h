#ifndef NSAPPLINKRPC_ONBUTTONEVENTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"


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
