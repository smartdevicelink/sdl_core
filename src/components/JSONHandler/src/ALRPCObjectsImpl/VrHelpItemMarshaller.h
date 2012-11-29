#ifndef NSAPPLINKRPC_VRHELPITEMMARSHALLER_INCLUDE
#define NSAPPLINKRPC_VRHELPITEMMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/VrHelpItem.h"


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

  struct VrHelpItemMarshaller
  {
    static bool checkIntegrity(VrHelpItem& e);
    static bool checkIntegrityConst(const VrHelpItem& e);
  
    static bool fromString(const std::string& s,VrHelpItem& e);
    static const std::string toString(const VrHelpItem& e);
  
    static bool fromJSON(const Json::Value& s,VrHelpItem& e);
    static Json::Value toJSON(const VrHelpItem& e);
  };
}

#endif
