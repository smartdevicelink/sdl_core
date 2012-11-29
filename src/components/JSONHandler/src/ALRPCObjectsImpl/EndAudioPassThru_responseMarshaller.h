#ifndef NSAPPLINKRPC_ENDAUDIOPASSTHRU_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ENDAUDIOPASSTHRU_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/EndAudioPassThru_response.h"


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

  struct EndAudioPassThru_responseMarshaller
  {
    static bool checkIntegrity(EndAudioPassThru_response& e);
    static bool checkIntegrityConst(const EndAudioPassThru_response& e);
  
    static bool fromString(const std::string& s,EndAudioPassThru_response& e);
    static const std::string toString(const EndAudioPassThru_response& e);
  
    static bool fromJSON(const Json::Value& s,EndAudioPassThru_response& e);
    static Json::Value toJSON(const EndAudioPassThru_response& e);
  };
}

#endif
