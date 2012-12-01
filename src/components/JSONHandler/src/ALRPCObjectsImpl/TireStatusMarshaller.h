#ifndef NSAPPLINKRPC_TIRESTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/TireStatus.h"


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

  struct TireStatusMarshaller
  {
    static bool checkIntegrity(TireStatus& e);
    static bool checkIntegrityConst(const TireStatus& e);
  
    static bool fromString(const std::string& s,TireStatus& e);
    static const std::string toString(const TireStatus& e);
  
    static bool fromJSON(const Json::Value& s,TireStatus& e);
    static Json::Value toJSON(const TireStatus& e);
  };
}

#endif
