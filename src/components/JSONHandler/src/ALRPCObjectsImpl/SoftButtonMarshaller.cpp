#include "../include/JSONHandler/ALRPCObjects/SoftButton.h"
#include "ImageMarshaller.h"
#include "SoftButtonTypeMarshaller.h"
#include "SystemActionMarshaller.h"

#include "SoftButtonMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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
  if(!ImageMarshaller::checkIntegrityConst(s.image))  return false;
  if(s.softButtonID>65536)  return false;
  if(!SystemActionMarshaller::checkIntegrityConst(s.systemAction))  return false;
  if(s.text.length()>500)  return false;
  if(!SoftButtonTypeMarshaller::checkIntegrityConst(s.type))  return false;
  return true;
}

Json::Value SoftButtonMarshaller::toJSON(const SoftButton& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["image"]=ImageMarshaller::toJSON(e.image);

  json["isHighlighted"]=Json::Value(e.isHighlighted);

  json["softButtonID"]=Json::Value(e.softButtonID);

  json["systemAction"]=SystemActionMarshaller::toJSON(e.systemAction);

  json["text"]=Json::Value(e.text);

  json["type"]=SoftButtonTypeMarshaller::toJSON(e.type);


  return json;
}


bool SoftButtonMarshaller::fromJSON(const Json::Value& json,SoftButton& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("image"))  return false;
    {
      const Json::Value& j=json["image"];
      if(!ImageMarshaller::fromJSON(j,c.image))
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
    if(!json.isMember("text"))  return false;
    {
      const Json::Value& j=json["text"];
      if(!j.isString())  return false;
      c.text=j.asString();
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

