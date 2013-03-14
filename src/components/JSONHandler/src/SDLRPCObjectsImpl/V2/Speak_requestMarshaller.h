#ifndef NSAPPLINKRPCV2_SPEAK_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SPEAK_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/Speak_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
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
