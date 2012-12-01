#include "../include/JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "ButtonNameMarshaller.h"
#include "ButtonPressModeMarshaller.h"

#include "OnButtonPressMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

