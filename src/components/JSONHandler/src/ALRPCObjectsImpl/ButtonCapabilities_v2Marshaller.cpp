#include "../include/JSONHandler/ALRPCObjects/ButtonCapabilities_v2.h"
#include "ButtonName_v2Marshaller.h"

#include "ButtonCapabilities_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool ButtonCapabilities_v2Marshaller::checkIntegrity(ButtonCapabilities_v2& s)
{
  return checkIntegrityConst(s);
}


bool ButtonCapabilities_v2Marshaller::fromString(const std::string& s,ButtonCapabilities_v2& e)
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


const std::string ButtonCapabilities_v2Marshaller::toString(const ButtonCapabilities_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ButtonCapabilities_v2Marshaller::checkIntegrityConst(const ButtonCapabilities_v2& s)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(s.name))  return false;
  return true;
}

Json::Value ButtonCapabilities_v2Marshaller::toJSON(const ButtonCapabilities_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["longPressAvailable"]=Json::Value(e.longPressAvailable);

  json["name"]=ButtonName_v2Marshaller::toJSON(e.name);

  json["shortPressAvailable"]=Json::Value(e.shortPressAvailable);

  json["upDownAvailable"]=Json::Value(e.upDownAvailable);


  return json;
}


bool ButtonCapabilities_v2Marshaller::fromJSON(const Json::Value& json,ButtonCapabilities_v2& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("longPressAvailable"))  return false;
    {
      const Json::Value& j=json["longPressAvailable"];
      if(!j.isBool())  return false;
      c.longPressAvailable=j.asBool();
    }
    if(!json.isMember("name"))  return false;
    {
      const Json::Value& j=json["name"];
      if(!ButtonName_v2Marshaller::fromJSON(j,c.name))
        return false;
    }
    if(!json.isMember("shortPressAvailable"))  return false;
    {
      const Json::Value& j=json["shortPressAvailable"];
      if(!j.isBool())  return false;
      c.shortPressAvailable=j.asBool();
    }
    if(!json.isMember("upDownAvailable"))  return false;
    {
      const Json::Value& j=json["upDownAvailable"];
      if(!j.isBool())  return false;
      c.upDownAvailable=j.asBool();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

