#ifndef VEHICLEDATARESULTCODEMARSHALLER_INCLUDE
#define VEHICLEDATARESULTCODEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/VehicleDataResultCode.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


//! marshalling class for VehicleDataResultCode

class VehicleDataResultCodeMarshaller
{
public:

  static std::string toName(const VehicleDataResultCode& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(VehicleDataResultCode& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=VehicleDataResultCode::INVALID_ENUM;
  }

  static bool checkIntegrity(VehicleDataResultCode& e)		{ return e.mInternal!=VehicleDataResultCode::INVALID_ENUM; } 
  static bool checkIntegrityConst(const VehicleDataResultCode& e)	{ return e.mInternal!=VehicleDataResultCode::INVALID_ENUM; } 

  static bool fromString(const std::string& s,VehicleDataResultCode& e);
  static const std::string toString(const VehicleDataResultCode& e);

  static bool fromJSON(const Json::Value& s,VehicleDataResultCode& e);
  static Json::Value toJSON(const VehicleDataResultCode& e);

  static const char* getName(VehicleDataResultCode::VehicleDataResultCodeInternal e)
  {
     return (e>=0 && e<6) ? mHashTable[e].name : NULL;
  }

  static const VehicleDataResultCode::VehicleDataResultCodeInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[6];
};

#endif
