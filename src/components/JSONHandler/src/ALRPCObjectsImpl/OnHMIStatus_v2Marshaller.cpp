#include "../include/JSONHandler/ALRPCObjects/OnHMIStatus_v2.h"
#include "AudioStreamingState_v2Marshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContext_v2Marshaller.h"

#include "OnHMIStatus_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnHMIStatus_v2Marshaller::checkIntegrity(OnHMIStatus_v2& s)
{
  return checkIntegrityConst(s);
}


bool OnHMIStatus_v2Marshaller::fromString(const std::string& s,OnHMIStatus_v2& e)
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


const std::string OnHMIStatus_v2Marshaller::toString(const OnHMIStatus_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnHMIStatus_v2Marshaller::checkIntegrityConst(const OnHMIStatus_v2& s)
{
  if(!HMILevelMarshaller::checkIntegrityConst(s.hmiLevel))  return false;
  if(!AudioStreamingState_v2Marshaller::checkIntegrityConst(s.audioStreamingState))  return false;
  if(!SystemContext_v2Marshaller::checkIntegrityConst(s.systemContext))  return false;
  return true;
}

Json::Value OnHMIStatus_v2Marshaller::toJSON(const OnHMIStatus_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnHMIStatus_v2");

  Json::Value j=Json::Value(Json::objectValue);

  j["hmiLevel"]=HMILevelMarshaller::toJSON(e.hmiLevel);

  j["audioStreamingState"]=AudioStreamingState_v2Marshaller::toJSON(e.audioStreamingState);

  j["systemContext"]=SystemContext_v2Marshaller::toJSON(e.systemContext);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnHMIStatus_v2Marshaller::fromJSON(const Json::Value& js,OnHMIStatus_v2& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnHMIStatus_v2"))  return false;

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
      if(!AudioStreamingState_v2Marshaller::fromJSON(j,c.audioStreamingState))
        return false;
    }
    if(!json.isMember("systemContext"))  return false;
    {
      const Json::Value& j=json["systemContext"];
      if(!SystemContext_v2Marshaller::fromJSON(j,c.systemContext))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

