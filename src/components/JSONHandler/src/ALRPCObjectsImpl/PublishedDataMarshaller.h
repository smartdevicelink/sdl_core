#ifndef PUBLISHEDDATAMARSHALLER_INCLUDE
#define PUBLISHEDDATAMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"
#include "../../include/JSONHandler/ALRPCObjects/PublishedData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct PublishedDataMarshaller
{
  static std::string toName(const PublishedData& e) 	{ return getName(e.mInternal.mDiscriminator) ?: ""; }

  static bool fromName(PublishedData::eDiscriminator& e,const std::string& s)
  { 
    return (e=getIndex(s.c_str()))!=PublishedData::INVALID_ENUM;
  }

  static bool checkIntegrity(PublishedData& e);
  static bool checkIntegrityConst(const PublishedData& e);

  static bool fromString(const std::string& s,PublishedData& e);
  static const std::string toString(const PublishedData& e);

  static bool fromJSON(const Json::Value& s,PublishedData& e);
  static Json::Value toJSON(const PublishedData& e);


  static const char* getName(PublishedData::eDiscriminator e)
  {
     return (e>=0 && e<17) ? mHashTable[e].name : NULL;
  }
   
  static const PublishedData::eDiscriminator getIndex(const char* s);

  static const PerfectHashTable mHashTable[17];
};
#endif
