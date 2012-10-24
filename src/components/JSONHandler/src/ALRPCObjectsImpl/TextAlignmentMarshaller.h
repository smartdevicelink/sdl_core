#ifndef TEXTALIGNMENTMARSHALLER_INCLUDE
#define TEXTALIGNMENTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/TextAlignment.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


//! marshalling class for TextAlignment

class TextAlignmentMarshaller
{
public:

  static std::string toName(const TextAlignment& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(TextAlignment& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=TextAlignment::INVALID_ENUM;
  }

  static bool checkIntegrity(TextAlignment& e)		{ return e.mInternal!=TextAlignment::INVALID_ENUM; } 
  static bool checkIntegrityConst(const TextAlignment& e)	{ return e.mInternal!=TextAlignment::INVALID_ENUM; } 

  static bool fromString(const std::string& s,TextAlignment& e);
  static const std::string toString(const TextAlignment& e);

  static bool fromJSON(const Json::Value& s,TextAlignment& e);
  static Json::Value toJSON(const TextAlignment& e);

  static const char* getName(TextAlignment::TextAlignmentInternal e)
  {
     return (e>=0 && e<3) ? mHashTable[e].name : NULL;
  }

  static const TextAlignment::TextAlignmentInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[3];
};

#endif
