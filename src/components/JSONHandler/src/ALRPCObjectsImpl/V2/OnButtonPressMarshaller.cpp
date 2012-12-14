#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonPress.h"
#include "ButtonNameMarshaller.h"
#include "ButtonPressModeMarshaller.h"

#include "OnButtonPressMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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
  if(s.customButtonID>65536)  return false;
  return true;
}

Json::Value OnButtonPressMarshaller::toJSON(const OnButtonPress& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  json["buttonPressMode"]=ButtonPressModeMarshaller::toJSON(e.buttonPressMode);

  json["customButtonID"]=Json::Value(e.customButtonID);

  return json;
}


bool OnButtonPressMarshaller::fromJSON(const Json::Value& json,OnButtonPress& c)
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
    if(!json.isMember("buttonPressMode"))  return false;
    {
      const Json::Value& j=json["buttonPressMode"];
      if(!ButtonPressModeMarshaller::fromJSON(j,c.buttonPressMode))
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

