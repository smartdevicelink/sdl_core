#ifndef DATATYPEMARSHALLER_INCLUDE
#define DATATYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/DataType.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


//! marshalling class for DataType

class DataTypeMarshaller
{
public:

  static std::string toName(const DataType& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(DataType& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=DataType::INVALID_ENUM;
  }

  static bool checkIntegrity(DataType& e)		{ return e.mInternal!=DataType::INVALID_ENUM; } 
  static bool checkIntegrityConst(const DataType& e)	{ return e.mInternal!=DataType::INVALID_ENUM; } 

  static bool fromString(const std::string& s,DataType& e);
  static const std::string toString(const DataType& e);

  static bool fromJSON(const Json::Value& s,DataType& e);
  static Json::Value toJSON(const DataType& e);

  static const char* getName(DataType::DataTypeInternal e)
  {
     return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }

  static const DataType::DataTypeInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[2];
};

#endif
