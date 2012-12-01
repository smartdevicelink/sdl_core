#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_v2_request.h"
#include "StartTime_v2Marshaller.h"
#include "UpdateMode_v2Marshaller.h"

#include "SetMediaClockTimer_v2_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool SetMediaClockTimer_v2_requestMarshaller::checkIntegrity(SetMediaClockTimer_v2_request& s)
{
  return checkIntegrityConst(s);
}


bool SetMediaClockTimer_v2_requestMarshaller::fromString(const std::string& s,SetMediaClockTimer_v2_request& e)
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


const std::string SetMediaClockTimer_v2_requestMarshaller::toString(const SetMediaClockTimer_v2_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetMediaClockTimer_v2_requestMarshaller::checkIntegrityConst(const SetMediaClockTimer_v2_request& s)
{
  if(s.startTime && !StartTime_v2Marshaller::checkIntegrityConst(*s.startTime))  return false;
  if(s.updateMode && !UpdateMode_v2Marshaller::checkIntegrityConst(*s.updateMode))  return false;
  return true;
}

Json::Value SetMediaClockTimer_v2_requestMarshaller::toJSON(const SetMediaClockTimer_v2_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("SetMediaClockTimer_v2");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.startTime)
    j["startTime"]=StartTime_v2Marshaller::toJSON(*e.startTime);

  if(e.updateMode)
    j["updateMode"]=UpdateMode_v2Marshaller::toJSON(*e.updateMode);

  json["request"]["parameters"]=j;
  return json;
}


bool SetMediaClockTimer_v2_requestMarshaller::fromJSON(const Json::Value& js,SetMediaClockTimer_v2_request& c)
{
  if(c.startTime)  delete c.startTime;
  c.startTime=0;

  if(c.updateMode)  delete c.updateMode;
  c.updateMode=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("SetMediaClockTimer_v2"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("startTime"))
    {
      const Json::Value& j=json["startTime"];
      c.startTime=new StartTime_v2();
      if(!StartTime_v2Marshaller::fromJSON(j,c.startTime[0]))
        return false;
    }
    if(json.isMember("updateMode"))
    {
      const Json::Value& j=json["updateMode"];
      c.updateMode=new UpdateMode_v2();
      if(!UpdateMode_v2Marshaller::fromJSON(j,c.updateMode[0]))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

