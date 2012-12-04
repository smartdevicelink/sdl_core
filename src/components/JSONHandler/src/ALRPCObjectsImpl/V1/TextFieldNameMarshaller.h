#ifndef NSAPPLINKRPCV1_TEXTFIELDNAMEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_TEXTFIELDNAMEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TextFieldName.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
{

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
  
}

#endif
