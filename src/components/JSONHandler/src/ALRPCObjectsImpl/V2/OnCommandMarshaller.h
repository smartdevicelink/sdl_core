#ifndef NSAPPLINKRPCV2_ONCOMMANDMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnCommand.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct OnCommandMarshaller
  {
    static bool checkIntegrity(OnCommand& e);
    static bool checkIntegrityConst(const OnCommand& e);
  
    static bool fromString(const std::string& s,OnCommand& e);
    static const std::string toString(const OnCommand& e);
  
    static bool fromJSON(const Json::Value& s,OnCommand& e);
    static Json::Value toJSON(const OnCommand& e);
  };
}

#endif
