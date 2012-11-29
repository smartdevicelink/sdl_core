#ifndef NSAPPLINKRPC_IMAGEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_IMAGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/Image.h"


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

  struct ImageMarshaller
  {
    static bool checkIntegrity(Image& e);
    static bool checkIntegrityConst(const Image& e);
  
    static bool fromString(const std::string& s,Image& e);
    static const std::string toString(const Image& e);
  
    static bool fromJSON(const Json::Value& s,Image& e);
    static Json::Value toJSON(const Image& e);
  };
}

#endif
