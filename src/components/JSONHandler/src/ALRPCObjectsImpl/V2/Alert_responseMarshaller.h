#ifndef NSAPPLINKRPCV2_ALERT_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ALERT_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/Alert_response.h"


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
