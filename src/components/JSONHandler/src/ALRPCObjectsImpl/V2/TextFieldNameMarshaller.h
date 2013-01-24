#ifndef NSAPPLINKRPCV2_TEXTFIELDNAMEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_TEXTFIELDNAMEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/TextFieldName.h"


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
       return (e>=0 && e<20) ? mHashTable[e].name : NULL;
    }
  
    static const TextFieldName::TextFieldNameInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[20];
  };
  
}

#endif
