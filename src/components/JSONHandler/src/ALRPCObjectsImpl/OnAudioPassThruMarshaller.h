#ifndef ONAUDIOPASSTHRUMARSHALLER_INCLUDE
#define ONAUDIOPASSTHRUMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnAudioPassThru.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnAudioPassThruMarshaller
{
  static bool checkIntegrity(OnAudioPassThru& e);
  static bool checkIntegrityConst(const OnAudioPassThru& e);

  static bool fromString(const std::string& s,OnAudioPassThru& e);
  static const std::string toString(const OnAudioPassThru& e);

  static bool fromJSON(const Json::Value& s,OnAudioPassThru& e);
  static Json::Value toJSON(const OnAudioPassThru& e);
};
#endif
