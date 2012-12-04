#ifndef NSAPPLINKRPCV2_ONBUTTONPRESSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONBUTTONPRESSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonPress.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
