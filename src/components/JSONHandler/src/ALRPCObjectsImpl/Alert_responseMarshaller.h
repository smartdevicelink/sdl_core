#ifndef NSAPPLINKRPC_ALERT_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ALERT_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/Alert_response.h"


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

  struct Alert_responseMarshaller
  {
    static bool checkIntegrity(Alert_response& e);
    static bool checkIntegrityConst(const Alert_response& e);
  
    static bool fromString(const std::string& s,Alert_response& e);
    static const std::string toString(const Alert_response& e);
  
    static bool fromJSON(const Json::Value& s,Alert_response& e);
    static Json::Value toJSON(const Alert_response& e);
  };
}

#endif
