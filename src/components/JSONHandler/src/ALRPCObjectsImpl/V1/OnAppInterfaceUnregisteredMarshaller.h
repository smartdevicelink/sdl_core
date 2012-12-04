#ifndef NSAPPLINKRPCV1_ONAPPINTERFACEUNREGISTEREDMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_ONAPPINTERFACEUNREGISTEREDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/OnAppInterfaceUnregistered.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
