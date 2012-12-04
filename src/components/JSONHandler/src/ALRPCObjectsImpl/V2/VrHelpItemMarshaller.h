#ifndef NSAPPLINKRPCV2_VRHELPITEMMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_VRHELPITEMMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/VrHelpItem.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
