#ifndef NSAPPLINKRPCV2_TTSCHUNKMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_TTSCHUNKMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct TTSChunkMarshaller
  {
    static bool checkIntegrity(TTSChunk& e);
    static bool checkIntegrityConst(const TTSChunk& e);
  
    static bool fromString(const std::string& s,TTSChunk& e);
    static const std::string toString(const TTSChunk& e);
  
    static bool fromJSON(const Json::Value& s,TTSChunk& e);
    static Json::Value toJSON(const TTSChunk& e);
  };
}

#endif
