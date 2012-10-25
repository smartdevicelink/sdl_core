#include "../../include/JSONHandler/ALRPCObjects/OnHMIStatus.h"
#include "AudioStreamingStateMarshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContextMarshaller.h"

#include "OnHMIStatusMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


bool OnHMIStatusMarshaller::checkIntegrity(OnHMIStatus& s)
{
  return checkIntegrityConst(s);
}


bool OnHMIStatusMarshaller::fromString(const std::string& s,OnHMIStatus& e)
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


const std::string OnHMIStatusMarshaller::toString(const OnHMIStatus& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnHMIStatusMarshaller::checkIntegrityConst(const OnHMIStatus& s)
{
  if(!HMILevelMarshaller::checkIntegrityConst(s.hmiLevel))  return false;
  if(!AudioStreamingStateMarshaller::checkIntegrityConst(s.audioStreamingState))  return false;
  if(!SystemContextMarshaller::checkIntegrityConst(s.systemContext))  return false;
  return true;
}

Json::Value OnHMIStatusMarshaller::toJSON(const OnHMIStatus& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnHMIStatus");

  Json::Value j=Json::Value(Json::objectValue);

  j["hmiLevel"]=HMILevelMarshaller::toJSON(e.hmiLevel);

  j["audioStreamingState"]=AudioStreamingStateMarshaller::toJSON(e.audioStreamingState);

  j["systemContext"]=SystemContextMarshaller::toJSON(e.systemContext);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnHMIStatusMarshaller::fromJSON(const Json::Value& js,OnHMIStatus& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnHMIStatus"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("hmiLevel"))  return false;
    {
      const Json::Value& j=json["hmiLevel"];
      if(!HMILevelMarshaller::fromJSON(j,c.hmiLevel))
        return false;
    }
    if(!json.isMember("audioStreamingState"))  return false;
    {
      const Json::Value& j=json["audioStreamingState"];
      if(!AudioStreamingStateMarshaller::fromJSON(j,c.audioStreamingState))
        return false;
    }
    if(!json.isMember("systemContext"))  return false;
    {
      const Json::Value& j=json["systemContext"];
      if(!SystemContextMarshaller::fromJSON(j,c.systemContext))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

