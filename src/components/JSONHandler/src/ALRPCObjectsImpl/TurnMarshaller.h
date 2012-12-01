#ifndef NSAPPLINKRPC_TURNMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TURNMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/Turn.h"


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

  struct TurnMarshaller
  {
    static bool checkIntegrity(Turn& e);
    static bool checkIntegrityConst(const Turn& e);
  
    static bool fromString(const std::string& s,Turn& e);
    static const std::string toString(const Turn& e);
  
    static bool fromJSON(const Json::Value& s,Turn& e);
    static Json::Value toJSON(const Turn& e);
  };
}

#endif
