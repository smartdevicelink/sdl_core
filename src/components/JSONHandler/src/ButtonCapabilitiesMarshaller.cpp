#include "JSONHandler/ButtonCapabilitiesMarshaller.h"
#include "JSONHandler/ButtonNameMarshaller.hh"
#include <json/reader.h>
#include <json/writer.h>

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
  return writer.write(toJSON(e));
}


Json::Value ButtonCapabilitiesMarshaller::toJSON(const ButtonCapabilities& e)
{  
  Json::Value json(Json::objectValue);
  
  json["name"]=ButtonNameMarshaller::toJSON(e.mName);  
  json["shortPressAvailable"] = Json::Value(e.mShortPressAvailable);
  json["longPressAvailable"] = Json::Value(e.mLongPressAvailable);
  json["upDownAvailable"] = Json::Value(e.mUpDownAvailable);

  return json;
}


bool ButtonCapabilitiesMarshaller::fromJSON(const Json::Value& json,ButtonCapabilities& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("name"))  return false;
    if (!ButtonNameMarshaller::fromJSON(json["name"], c.mName)) return false;

    if(!json.isMember("shortPressAvailable")) return false;
    c.mShortPressAvailable = json["shortPressAvailable"].asBool();

    if(!json.isMember("longPressAvailable")) return false;
    c.mLongPressAvailable = json["longPressAvailable"].asBool();

    if(!json.isMember("upDownAvailable")) return false;
    c.mUpDownAvailable = json["upDownAvailable"].asBool();    
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

