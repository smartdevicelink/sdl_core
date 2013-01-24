#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonEvent.h"
#include "ButtonEventModeMarshaller.h"
#include "ButtonNameMarshaller.h"

#include "OnButtonEventMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  json["buttonEventMode"]=ButtonEventModeMarshaller::toJSON(e.buttonEventMode);

  json["customButtonID"]=Json::Value(e.customButtonID);

  return json;
}


bool OnButtonEventMarshaller::fromJSON(const Json::Value& json,OnButtonEvent& c)
{
  try
  {
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

