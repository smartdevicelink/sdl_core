#ifndef NSAPPLINKRPC_DTCMARSHALLER_INCLUDE
#define NSAPPLINKRPC_DTCMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/DTC.h"


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

  struct DTCMarshaller
  {
    static bool checkIntegrity(DTC& e);
    static bool checkIntegrityConst(const DTC& e);
  
    static bool fromString(const std::string& s,DTC& e);
    static const std::string toString(const DTC& e);
  
    static bool fromJSON(const Json::Value& s,DTC& e);
    static Json::Value toJSON(const DTC& e);
  };
}

#endif
