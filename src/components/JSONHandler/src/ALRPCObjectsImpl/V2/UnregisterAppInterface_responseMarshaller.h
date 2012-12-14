#ifndef NSAPPLINKRPCV2_UNREGISTERAPPINTERFACE_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_UNREGISTERAPPINTERFACE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/UnregisterAppInterface_response.h"


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

  struct UnregisterAppInterface_responseMarshaller
  {
    static bool checkIntegrity(UnregisterAppInterface_response& e);
    static bool checkIntegrityConst(const UnregisterAppInterface_response& e);
  
    static bool fromString(const std::string& s,UnregisterAppInterface_response& e);
    static const std::string toString(const UnregisterAppInterface_response& e);
  
    static bool fromJSON(const Json::Value& s,UnregisterAppInterface_response& e);
    static Json::Value toJSON(const UnregisterAppInterface_response& e);
  };
}

#endif
