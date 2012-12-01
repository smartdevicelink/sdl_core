#ifndef NSAPPLINKRPC_SOFTBUTTONMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SOFTBUTTONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/SoftButton.h"


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

  struct SoftButtonMarshaller
  {
    static bool checkIntegrity(SoftButton& e);
    static bool checkIntegrityConst(const SoftButton& e);
  
    static bool fromString(const std::string& s,SoftButton& e);
    static const std::string toString(const SoftButton& e);
  
    static bool fromJSON(const Json::Value& s,SoftButton& e);
    static Json::Value toJSON(const SoftButton& e);
  };
}

#endif
