#ifndef NSAPPLINKRPCV2_ONBUTTONEVENTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonEvent.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
