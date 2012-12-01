#include "../include/JSONHandler/ALRPCObjects/StartTime_v2.h"


#include "StartTime_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool StartTime_v2Marshaller::checkIntegrity(StartTime_v2& s)
{
  return checkIntegrityConst(s);
}


bool StartTime_v2Marshaller::fromString(const std::string& s,StartTime_v2& e)
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


const std::string StartTime_v2Marshaller::toString(const StartTime_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool StartTime_v2Marshaller::checkIntegrityConst(const StartTime_v2& s)
{
  if(s.hours>59)  return false;
  if(s.minutes>59)  return false;
  if(s.seconds>59)  return false;
  return true;
}

Json::Value StartTime_v2Marshaller::toJSON(const StartTime_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["hours"]=Json::Value(e.hours);

  json["minutes"]=Json::Value(e.minutes);

  json["seconds"]=Json::Value(e.seconds);


  return json;
}


bool StartTime_v2Marshaller::fromJSON(const Json::Value& json,StartTime_v2& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("hours"))  return false;
    {
      const Json::Value& j=json["hours"];
      if(!j.isInt())  return false;
      c.hours=j.asInt();
    }
    if(!json.isMember("minutes"))  return false;
    {
      const Json::Value& j=json["minutes"];
      if(!j.isInt())  return false;
      c.minutes=j.asInt();
    }
    if(!json.isMember("seconds"))  return false;
    {
      const Json::Value& j=json["seconds"];
      if(!j.isInt())  return false;
      c.seconds=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

