#ifndef NSAPPLINKRPCV2_IMAGEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_IMAGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
