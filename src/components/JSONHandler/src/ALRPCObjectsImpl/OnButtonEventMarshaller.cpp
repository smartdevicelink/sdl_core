#include "../../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "ButtonEventModeMarshaller.h"
#include "ButtonNameMarshaller.h"

#include "OnButtonEventMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool OnButtonEventMarshaller::checkIntegrity(OnButtonEvent& s)
{
  return checkIntegrityConst(s);
}


bool OnButtonEventMarshaller::fromString(const std::string& s,OnButtonEvent& e)
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


const std::string OnButtonEventMarshaller::toString(const OnButtonEvent& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnButtonEventMarshaller::checkIntegrityConst(const OnButtonEvent& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.buttonName))  return false;
  if(!ButtonEventModeMarshaller::checkIntegrityConst(s.buttonEventMode))  return false;
  if(s.customButtonID>65536)  return false;
  return true;
}

Json::Value OnButtonEventMarshaller::toJSON(const OnButtonEvent& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnButtonEvent");

  Json::Value j=Json::Value(Json::objectValue);

  j["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  j["buttonEventMode"]=ButtonEventModeMarshaller::toJSON(e.buttonEventMode);

  j["customButtonID"]=Json::Value(e.customButtonID);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnButtonEventMarshaller::fromJSON(const Json::Value& js,OnButtonEvent& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnButtonEvent"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonNameMarshaller::fromJSON(j,c.buttonName))
        return false;
    }
    if(!json.isMember("buttonEventMode"))  return false;
    {
      const Json::Value& j=json["buttonEventMode"];
      if(!ButtonEventModeMarshaller::fromJSON(j,c.buttonEventMode))
        return false;
    }
    if(!json.isMember("customButtonID"))  return false;
    {
      const Json::Value& j=json["customButtonID"];
      if(!j.isInt())  return false;
      c.customButtonID=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

