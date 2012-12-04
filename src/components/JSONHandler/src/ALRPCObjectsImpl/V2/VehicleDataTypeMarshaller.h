#ifndef NSAPPLINKRPCV2_VEHICLEDATATYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_VEHICLEDATATYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleDataType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for VehicleDataType

  class VehicleDataTypeMarshaller
  {
  public:
  
    static std::string toName(const VehicleDataType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(VehicleDataType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=VehicleDataType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(VehicleDataType& e)		{ return e.mInternal!=VehicleDataType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const VehicleDataType& e)	{ return e.mInternal!=VehicleDataType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,VehicleDataType& e);
    static const std::string toString(const VehicleDataType& e);
  
    static bool fromJSON(const Json::Value& s,VehicleDataType& e);
    static Json::Value toJSON(const VehicleDataType& e);
  
    static const char* getName(VehicleDataType::VehicleDataTypeInternal e)
    {
       return (e>=0 && e<16) ? mHashTable[e].name : NULL;
    }
  
    static const VehicleDataType::VehicleDataTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[16];
  };
  
}

#endif
