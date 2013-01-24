#ifndef NSAPPLINKRPCV2_UNREGISTERAPPINTERFACE_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_UNREGISTERAPPINTERFACE_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/UnregisterAppInterface_request.h"


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

  struct UnregisterAppInterface_requestMarshaller
  {
    static bool checkIntegrity(UnregisterAppInterface_request& e);
    static bool checkIntegrityConst(const UnregisterAppInterface_request& e);
  
    static bool fromString(const std::string& s,UnregisterAppInterface_request& e);
    static const std::string toString(const UnregisterAppInterface_request& e);
  
    static bool fromJSON(const Json::Value& s,UnregisterAppInterface_request& e);
    static Json::Value toJSON(const UnregisterAppInterface_request& e);
  };
}

#endif
