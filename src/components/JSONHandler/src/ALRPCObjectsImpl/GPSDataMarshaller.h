#ifndef GPSDATAMARSHALLER_INCLUDE
#define GPSDATAMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/GPSData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct GPSDataMarshaller
{
  static bool checkIntegrity(GPSData& e);
  static bool checkIntegrityConst(const GPSData& e);

  static bool fromString(const std::string& s,GPSData& e);
  static const std::string toString(const GPSData& e);

  static bool fromJSON(const Json::Value& s,GPSData& e);
  static Json::Value toJSON(const GPSData& e);
};
#endif
