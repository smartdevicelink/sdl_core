//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMERMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMERMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimer.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetMediaClockTimerMarshaller
    {
      static bool checkIntegrity(SetMediaClockTimer& e);
      static bool checkIntegrityConst(const SetMediaClockTimer& e);
    
      static bool fromString(const std::string& s,SetMediaClockTimer& e);
      static const std::string toString(const SetMediaClockTimer& e);
    
      static bool fromJSON(const Json::Value& s,SetMediaClockTimer& e);
      static Json::Value toJSON(const SetMediaClockTimer& e);
    };
  }
}

#endif
