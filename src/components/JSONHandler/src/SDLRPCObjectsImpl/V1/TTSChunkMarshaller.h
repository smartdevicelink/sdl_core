#ifndef NSAPPLINKRPC_TTSCHUNKMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TTSCHUNKMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
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
