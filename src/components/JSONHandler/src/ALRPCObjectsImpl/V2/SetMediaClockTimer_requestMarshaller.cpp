#include "../include/JSONHandler/ALRPCObjects/V2/SetMediaClockTimer_request.h"
#include "StartTimeMarshaller.h"
#include "UpdateModeMarshaller.h"

#include "SetMediaClockTimer_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool SetMediaClockTimer_requestMarshaller::checkIntegrity(SetMediaClockTimer_request& s)
{
  return checkIntegrityConst(s);
}


bool SetMediaClockTimer_requestMarshaller::fromString(const std::string& s,SetMediaClockTimer_request& e)
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


const std::string SetMediaClockTimer_requestMarshaller::toString(const SetMediaClockTimer_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetMediaClockTimer_requestMarshaller::checkIntegrityConst(const SetMediaClockTimer_request& s)
{
  if(s.startTime && !StartTimeMarshaller::checkIntegrityConst(*s.startTime))  return false;
  if(!UpdateModeMarshaller::checkIntegrityConst(s.updateMode))  return false;
  return true;
}

Json::Value SetMediaClockTimer_requestMarshaller::toJSON(const SetMediaClockTimer_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  if(e.startTime)
    json["startTime"]=StartTimeMarshaller::toJSON(*e.startTime);

  json["updateMode"]=UpdateModeMarshaller::toJSON(e.updateMode);

  return json;
}


bool SetMediaClockTimer_requestMarshaller::fromJSON(const Json::Value& json,SetMediaClockTimer_request& c)
{
  if(c.startTime)  delete c.startTime;
  c.startTime=0;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("startTime"))
    {
      const Json::Value& j=json["startTime"];
      c.startTime=new StartTime();
      if(!StartTimeMarshaller::fromJSON(j,c.startTime[0]))
        return false;
    }
    if(!json.isMember("updateMode"))  return false;
    {
      const Json::Value& j=json["updateMode"];
      if(!UpdateModeMarshaller::fromJSON(j,c.updateMode))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

