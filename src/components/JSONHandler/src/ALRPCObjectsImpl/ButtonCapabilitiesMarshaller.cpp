#include "../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include "ButtonNameMarshaller.h"

#include "ButtonCapabilitiesMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;


bool ButtonCapabilitiesMarshaller::checkIntegrity(ButtonCapabilities& s)
{
  return checkIntegrityConst(s);
}


bool ButtonCapabilitiesMarshaller::fromString(const std::string& s,ButtonCapabilities& e)
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


const std::string ButtonCapabilitiesMarshaller::toString(const ButtonCapabilities& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ButtonCapabilitiesMarshaller::checkIntegrityConst(const ButtonCapabilities& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.name))  return false;
  return true;
}

Json::Value ButtonCapabilitiesMarshaller::toJSON(const ButtonCapabilities& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["longPressAvailable"]=Json::Value(e.longPressAvailable);

  json["name"]=ButtonNameMarshaller::toJSON(e.name);

  json["shortPressAvailable"]=Json::Value(e.shortPressAvailable);

  json["upDownAvailable"]=Json::Value(e.upDownAvailable);


  return json;
}


bool ButtonCapabilitiesMarshaller::fromJSON(const Json::Value& json,ButtonCapabilities& c)
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
      if(!ButtonNameMarshaller::fromJSON(j,c.name))
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

