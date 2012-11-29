#ifndef NSAPPLINKRPC_PERFORMINTERACTION_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_PERFORMINTERACTION_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"


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

  struct PerformInteraction_responseMarshaller
  {
    static bool checkIntegrity(PerformInteraction_response& e);
    static bool checkIntegrityConst(const PerformInteraction_response& e);
  
    static bool fromString(const std::string& s,PerformInteraction_response& e);
    static const std::string toString(const PerformInteraction_response& e);
  
    static bool fromJSON(const Json::Value& s,PerformInteraction_response& e);
    static Json::Value toJSON(const PerformInteraction_response& e);
  };
}

#endif
