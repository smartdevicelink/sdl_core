#ifndef TTSCHUNKMARSHALLER_INCLUDE
#define TTSCHUNKMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct TTSChunkMarshaller
{
  static bool checkIntegrity(TTSChunk& e);
  static bool checkIntegrityConst(const TTSChunk& e);

  static bool fromString(const std::string& s,TTSChunk& e);
  static const std::string toString(const TTSChunk& e);

  static bool fromJSON(const Json::Value& s,TTSChunk& e);
  static Json::Value toJSON(const TTSChunk& e);
};
#endif
