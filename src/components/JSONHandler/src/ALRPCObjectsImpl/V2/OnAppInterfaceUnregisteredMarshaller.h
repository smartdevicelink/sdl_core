#ifndef NSAPPLINKRPCV2_ONAPPINTERFACEUNREGISTEREDMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONAPPINTERFACEUNREGISTEREDMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnAppInterfaceUnregistered.h"


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

  struct OnAppInterfaceUnregisteredMarshaller
  {
    static bool checkIntegrity(OnAppInterfaceUnregistered& e);
    static bool checkIntegrityConst(const OnAppInterfaceUnregistered& e);
  
    static bool fromString(const std::string& s,OnAppInterfaceUnregistered& e);
    static const std::string toString(const OnAppInterfaceUnregistered& e);
  
    static bool fromJSON(const Json::Value& s,OnAppInterfaceUnregistered& e);
    static Json::Value toJSON(const OnAppInterfaceUnregistered& e);
  };
}

#endif
