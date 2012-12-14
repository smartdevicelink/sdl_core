#ifndef NSAPPLINKRPCV2_SPEAK_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SPEAK_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/Speak_request.h"


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

  struct Speak_requestMarshaller
  {
    static bool checkIntegrity(Speak_request& e);
    static bool checkIntegrityConst(const Speak_request& e);
  
    static bool fromString(const std::string& s,Speak_request& e);
    static const std::string toString(const Speak_request& e);
  
    static bool fromJSON(const Json::Value& s,Speak_request& e);
    static Json::Value toJSON(const Speak_request& e);
  };
}

#endif
