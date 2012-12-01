#ifndef NSAPPLINKRPC_SLIDER_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SLIDER_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/Slider_response.h"


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

  struct Slider_responseMarshaller
  {
    static bool checkIntegrity(Slider_response& e);
    static bool checkIntegrityConst(const Slider_response& e);
  
    static bool fromString(const std::string& s,Slider_response& e);
    static const std::string toString(const Slider_response& e);
  
    static bool fromJSON(const Json::Value& s,Slider_response& e);
    static Json::Value toJSON(const Slider_response& e);
  };
}

#endif
