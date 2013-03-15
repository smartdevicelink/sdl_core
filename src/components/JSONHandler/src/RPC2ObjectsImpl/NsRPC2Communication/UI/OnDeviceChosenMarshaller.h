//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ONDEVICECHOSENMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDEVICECHOSENMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDeviceChosen.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct OnDeviceChosenMarshaller
    {
      static bool checkIntegrity(OnDeviceChosen& e);
      static bool checkIntegrityConst(const OnDeviceChosen& e);
    
      static bool fromString(const std::string& s,OnDeviceChosen& e);
      static const std::string toString(const OnDeviceChosen& e);
    
      static bool fromJSON(const Json::Value& s,OnDeviceChosen& e);
      static Json::Value toJSON(const OnDeviceChosen& e);
    };
  }
}

#endif
