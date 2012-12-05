#ifndef NSAPPLINKRPCV2_IMAGETYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_IMAGETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/ImageType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
