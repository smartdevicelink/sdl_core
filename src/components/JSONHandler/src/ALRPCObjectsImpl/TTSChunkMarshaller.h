#ifndef TTSCHUNKMARSHALLER_INCLUDE
#define TTSCHUNKMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
