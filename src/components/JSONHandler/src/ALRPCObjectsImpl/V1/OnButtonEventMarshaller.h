#ifndef NSAPPLINKRPCV1_ONBUTTONEVENTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/OnButtonEvent.h"


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
