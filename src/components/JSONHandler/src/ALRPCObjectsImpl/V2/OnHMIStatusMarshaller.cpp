#include "../include/JSONHandler/ALRPCObjects/V2/OnHMIStatus.h"
#include "AudioStreamingStateMarshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContextMarshaller.h"

#include "OnHMIStatusMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["hmiLevel"]=HMILevelMarshaller::toJSON(e.hmiLevel);

  json["audioStreamingState"]=AudioStreamingStateMarshaller::toJSON(e.audioStreamingState);

  json["systemContext"]=SystemContextMarshaller::toJSON(e.systemContext);

  return json;
}


bool OnHMIStatusMarshaller::fromJSON(const Json::Value& json,OnHMIStatus& c)
{
  try
  {
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

