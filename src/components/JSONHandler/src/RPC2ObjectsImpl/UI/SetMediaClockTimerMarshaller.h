#ifndef RPC2COMMUNICATION_UI_SETMEDIACLOCKTIMERMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_SETMEDIACLOCKTIMERMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/SetMediaClockTimer.h"

namespace RPC2Communication
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
