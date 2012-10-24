#ifndef IMAGEMARSHALLER_INCLUDE
#define IMAGEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct ImageMarshaller
{
  static bool checkIntegrity(Image& e);
  static bool checkIntegrityConst(const Image& e);

  static bool fromString(const std::string& s,Image& e);
  static const std::string toString(const Image& e);

  static bool fromJSON(const Json::Value& s,Image& e);
  static Json::Value toJSON(const Image& e);
};
#endif
