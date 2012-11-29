#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent_v2.h"
#include "ButtonEventModeMarshaller.h"
#include "ButtonName_v2Marshaller.h"

#include "OnButtonEvent_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnButtonEvent_v2Marshaller::checkIntegrity(OnButtonEvent_v2& s)
{
  return checkIntegrityConst(s);
}


bool OnButtonEvent_v2Marshaller::fromString(const std::string& s,OnButtonEvent_v2& e)
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


const std::string OnButtonEvent_v2Marshaller::toString(const OnButtonEvent_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnButtonEvent_v2Marshaller::checkIntegrityConst(const OnButtonEvent_v2& s)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(s.buttonName))  return false;
  if(!ButtonEventModeMarshaller::checkIntegrityConst(s.buttonEventMode))  return false;
  if(s.customButtonID>65536)  return false;
  return true;
}

Json::Value OnButtonEvent_v2Marshaller::toJSON(const OnButtonEvent_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnButtonEvent_v2");

  Json::Value j=Json::Value(Json::objectValue);

  j["buttonName"]=ButtonName_v2Marshaller::toJSON(e.buttonName);

  j["buttonEventMode"]=ButtonEventModeMarshaller::toJSON(e.buttonEventMode);

  j["customButtonID"]=Json::Value(e.customButtonID);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnButtonEvent_v2Marshaller::fromJSON(const Json::Value& js,OnButtonEvent_v2& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnButtonEvent_v2"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonName_v2Marshaller::fromJSON(j,c.buttonName))
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

