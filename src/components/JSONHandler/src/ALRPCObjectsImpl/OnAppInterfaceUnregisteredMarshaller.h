#ifndef ONAPPINTERFACEUNREGISTEREDMARSHALLER_INCLUDE
#define ONAPPINTERFACEUNREGISTEREDMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct OnAppInterfaceUnregisteredMarshaller
{
  static bool checkIntegrity(OnAppInterfaceUnregistered& e);
  static bool checkIntegrityConst(const OnAppInterfaceUnregistered& e);

  static bool fromString(const std::string& s,OnAppInterfaceUnregistered& e);
  static const std::string toString(const OnAppInterfaceUnregistered& e);

  static bool fromJSON(const Json::Value& s,OnAppInterfaceUnregistered& e);
  static Json::Value toJSON(const OnAppInterfaceUnregistered& e);
};
#endif
