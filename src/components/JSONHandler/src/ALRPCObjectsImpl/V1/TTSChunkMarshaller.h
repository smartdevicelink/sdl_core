#ifndef NSAPPLINKRPC_TTSCHUNKMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TTSCHUNKMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
