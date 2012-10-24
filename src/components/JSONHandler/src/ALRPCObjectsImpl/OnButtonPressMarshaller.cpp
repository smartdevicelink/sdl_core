#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "ButtonNameMarshaller.h"
#include "ButtonPressModeMarshaller.h"

#include "OnButtonPressMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool OnButtonPressMarshaller::checkIntegrity(OnButtonPress& s)
{
  return checkIntegrityConst(s);
}


bool OnButtonPressMarshaller::fromString(const std::string& s,OnButtonPress& e)
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


const std::string OnButtonPressMarshaller::toString(const OnButtonPress& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnButtonPressMarshaller::checkIntegrityConst(const OnButtonPress& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.buttonName))  return false;
  if(!ButtonPressModeMarshaller::checkIntegrityConst(s.buttonPressMode))  return false;
  if(s.customButtonName.length()>500)  return false;
  return true;
}

Json::Value OnButtonPressMarshaller::toJSON(const OnButtonPress& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnButtonPress");

  Json::Value j=Json::Value(Json::objectValue);

  j["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  j["buttonPressMode"]=ButtonPressModeMarshaller::toJSON(e.buttonPressMode);

  j["customButtonName"]=Json::Value(e.customButtonName);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnButtonPressMarshaller::fromJSON(const Json::Value& js,OnButtonPress& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnButtonPress"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonNameMarshaller::fromJSON(j,c.buttonName))
        return false;
    }
    if(!json.isMember("buttonPressMode"))  return false;
    {
      const Json::Value& j=json["buttonPressMode"];
      if(!ButtonPressModeMarshaller::fromJSON(j,c.buttonPressMode))
        return false;
    }
    if(!json.isMember("customButtonName"))  return false;
    {
      const Json::Value& j=json["customButtonName"];
      if(!j.isString())  return false;
      c.customButtonName=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

