#ifndef NSAPPLINKRPC_SETAPPICON_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SETAPPICON_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/SetAppIcon_request.h"


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
