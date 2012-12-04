#include "../include/JSONHandler/ALRPCObjects/V2/Slider_response.h"
#include "ResultMarshaller.h"

#include "Slider_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool Slider_responseMarshaller::checkIntegrity(Slider_response& s)
{
  return checkIntegrityConst(s);
}


bool Slider_responseMarshaller::fromString(const std::string& s,Slider_response& e)
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


const std::string Slider_responseMarshaller::toString(const Slider_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Slider_responseMarshaller::checkIntegrityConst(const Slider_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.sliderPosition>26)  return false;
  if(s.sliderPosition<1)  return false;
  return true;
}

Json::Value Slider_responseMarshaller::toJSON(const Slider_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  json["sliderPosition"]=Json::Value(e.sliderPosition);

  return json;
}


bool Slider_responseMarshaller::fromJSON(const Json::Value& json,Slider_response& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("success"))  return false;
    {
      const Json::Value& j=json["success"];
      if(!j.isBool())  return false;
      c.success=j.asBool();
    }
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      if(!ResultMarshaller::fromJSON(j,c.resultCode))
        return false;
    }
    if(!json.isMember("sliderPosition"))  return false;
    {
      const Json::Value& j=json["sliderPosition"];
      if(!j.isInt())  return false;
      c.sliderPosition=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

