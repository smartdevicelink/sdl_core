#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButton.h"
#include "ImageMarshaller.h"
#include "SoftButtonTypeMarshaller.h"
#include "SystemActionMarshaller.h"

#include "SoftButtonMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SoftButtonMarshaller::checkIntegrity(SoftButton& s)
{
  return checkIntegrityConst(s);
}


bool SoftButtonMarshaller::fromString(const std::string& s,SoftButton& e)
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


const std::string SoftButtonMarshaller::toString(const SoftButton& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SoftButtonMarshaller::checkIntegrityConst(const SoftButton& s)
{
  if(s.image && !ImageMarshaller::checkIntegrityConst(*s.image))  return false;
  if(s.softButtonID>65536)  return false;
  if(!SystemActionMarshaller::checkIntegrityConst(s.systemAction))  return false;
  if(s.text && s.text->length()>500)  return false;
  if(!SoftButtonTypeMarshaller::checkIntegrityConst(s.type))  return false;
  return true;
}

Json::Value SoftButtonMarshaller::toJSON(const SoftButton& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  if(e.image)
    json["image"]=ImageMarshaller::toJSON(*e.image);

  json["isHighlighted"]=Json::Value(e.isHighlighted);

  json["softButtonID"]=Json::Value(e.softButtonID);

  json["systemAction"]=SystemActionMarshaller::toJSON(e.systemAction);

  if(e.text)
    json["text"]=Json::Value(*e.text);

  json["type"]=SoftButtonTypeMarshaller::toJSON(e.type);


  return json;
}


bool SoftButtonMarshaller::fromJSON(const Json::Value& json,SoftButton& c)
{
  if(c.image)  delete c.image;
  c.image=0;

  if(c.text)  delete c.text;
  c.text=0;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("image"))
    {
      const Json::Value& j=json["image"];
      c.image=new Image();
      if(!ImageMarshaller::fromJSON(j,c.image[0]))
        return false;
    }
    if(!json.isMember("isHighlighted"))  return false;
    {
      const Json::Value& j=json["isHighlighted"];
      if(!j.isBool())  return false;
      c.isHighlighted=j.asBool();
    }
    if(!json.isMember("softButtonID"))  return false;
    {
      const Json::Value& j=json["softButtonID"];
      if(!j.isInt())  return false;
      c.softButtonID=j.asInt();
    }
    if(!json.isMember("systemAction"))  return false;
    {
      const Json::Value& j=json["systemAction"];
      if(!SystemActionMarshaller::fromJSON(j,c.systemAction))
        return false;
    }
    if(json.isMember("text"))
    {
      const Json::Value& j=json["text"];
      if(!j.isString())  return false;
      c.text=new std::string(j.asString());
    }
    if(!json.isMember("type"))  return false;
    {
      const Json::Value& j=json["type"];
      if(!SoftButtonTypeMarshaller::fromJSON(j,c.type))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

