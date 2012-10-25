#ifndef TEXTFIELDNAMEMARSHALLER_INCLUDE
#define TEXTFIELDNAMEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/TextFieldName.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


//! marshalling class for TextFieldName

class TextFieldNameMarshaller
{
public:

  static std::string toName(const TextFieldName& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(TextFieldName& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=TextFieldName::INVALID_ENUM;
  }

  static bool checkIntegrity(TextFieldName& e)		{ return e.mInternal!=TextFieldName::INVALID_ENUM; } 
  static bool checkIntegrityConst(const TextFieldName& e)	{ return e.mInternal!=TextFieldName::INVALID_ENUM; } 

  static bool fromString(const std::string& s,TextFieldName& e);
  static const std::string toString(const TextFieldName& e);

  static bool fromJSON(const Json::Value& s,TextFieldName& e);
  static Json::Value toJSON(const TextFieldName& e);

  static const char* getName(TextFieldName::TextFieldNameInternal e)
  {
     return (e>=0 && e<7) ? mHashTable[e].name : NULL;
  }

  static const TextFieldName::TextFieldNameInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[7];
};

#endif
