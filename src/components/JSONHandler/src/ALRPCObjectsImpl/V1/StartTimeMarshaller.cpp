#include "../include/JSONHandler/ALRPCObjects/V1/StartTime.h"


#include "StartTimeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;


bool StartTimeMarshaller::checkIntegrity(StartTime& s)
{
  return checkIntegrityConst(s);
}


bool StartTimeMarshaller::fromString(const std::string& s,StartTime& e)
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


const std::string StartTimeMarshaller::toString(const StartTime& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool StartTimeMarshaller::checkIntegrityConst(const StartTime& s)
{
  if(s.hours>59)  return false;
  if(s.minutes>59)  return false;
  if(s.seconds>59)  return false;
  return true;
}

Json::Value StartTimeMarshaller::toJSON(const StartTime& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["hours"]=Json::Value(e.hours);

  json["minutes"]=Json::Value(e.minutes);

  json["seconds"]=Json::Value(e.seconds);


  return json;
}


bool StartTimeMarshaller::fromJSON(const Json::Value& json,StartTime& c)
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

