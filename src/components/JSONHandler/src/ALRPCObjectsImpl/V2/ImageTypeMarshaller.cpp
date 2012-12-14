#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/ImageType.h"
#include "ImageTypeMarshaller.h"
#include "ImageTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const ImageType::ImageTypeInternal ImageTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return ImageType::INVALID_ENUM;
  const struct PerfectHashTable* p=ImageType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<ImageType::ImageTypeInternal>(p->idx) : ImageType::INVALID_ENUM;
}


bool ImageTypeMarshaller::fromJSON(const Json::Value& s,ImageType& e)
{
  e.mInternal=ImageType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=ImageType::INVALID_ENUM);
}


Json::Value ImageTypeMarshaller::toJSON(const ImageType& e)
{
  if(e.mInternal==ImageType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool ImageTypeMarshaller::fromString(const std::string& s,ImageType& e)
{
  e.mInternal=ImageType::INVALID_ENUM;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string ImageTypeMarshaller::toString(const ImageType& e)
{
  Json::FastWriter writer;
  return e.mInternal==ImageType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable ImageTypeMarshaller::mHashTable[2]=
{
  {"STATIC",0},
  {"DYNAMIC",1}
};
