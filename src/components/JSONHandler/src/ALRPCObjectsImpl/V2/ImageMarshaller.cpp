#include "../include/JSONHandler/ALRPCObjects/V2/Image.h"
#include "ImageTypeMarshaller.h"

#include "ImageMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool ImageMarshaller::checkIntegrity(Image& s)
{
  return checkIntegrityConst(s);
}


bool ImageMarshaller::fromString(const std::string& s,Image& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string ImageMarshaller::toString(const Image& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ImageMarshaller::checkIntegrityConst(const Image& s)
{
  if(!ImageTypeMarshaller::checkIntegrityConst(s.imageType))  return false;
  if(s.value.length()>65535)  return false;
  return true;
}

Json::Value ImageMarshaller::toJSON(const Image& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["imageType"]=ImageTypeMarshaller::toJSON(e.imageType);

  json["value"]=Json::Value(e.value);


  return json;
}


bool ImageMarshaller::fromJSON(const Json::Value& json,Image& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("imageType"))  return false;
    {
      const Json::Value& j=json["imageType"];
      if(!ImageTypeMarshaller::fromJSON(j,c.imageType))
        return false;
    }
    if(!json.isMember("value"))  return false;
    {
      const Json::Value& j=json["value"];
      if(!j.isString())  return false;
      c.value=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

