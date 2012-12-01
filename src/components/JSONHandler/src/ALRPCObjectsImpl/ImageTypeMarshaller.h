#ifndef NSAPPLINKRPC_IMAGETYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_IMAGETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/ImageType.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

//! marshalling class for ImageType

  class ImageTypeMarshaller
  {
  public:
  
    static std::string toName(const ImageType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(ImageType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=ImageType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(ImageType& e)		{ return e.mInternal!=ImageType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const ImageType& e)	{ return e.mInternal!=ImageType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,ImageType& e);
    static const std::string toString(const ImageType& e);
  
    static bool fromJSON(const Json::Value& s,ImageType& e);
    static Json::Value toJSON(const ImageType& e);
  
    static const char* getName(ImageType::ImageTypeInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const ImageType::ImageTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
