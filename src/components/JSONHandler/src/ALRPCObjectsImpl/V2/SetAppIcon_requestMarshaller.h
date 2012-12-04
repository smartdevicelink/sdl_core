#ifndef NSAPPLINKRPCV2_SETAPPICON_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SETAPPICON_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SetAppIcon_request.h"


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

  struct SetAppIcon_requestMarshaller
  {
    static bool checkIntegrity(SetAppIcon_request& e);
    static bool checkIntegrityConst(const SetAppIcon_request& e);
  
    static bool fromString(const std::string& s,SetAppIcon_request& e);
    static const std::string toString(const SetAppIcon_request& e);
  
    static bool fromJSON(const Json::Value& s,SetAppIcon_request& e);
    static Json::Value toJSON(const SetAppIcon_request& e);
  };
}

#endif
