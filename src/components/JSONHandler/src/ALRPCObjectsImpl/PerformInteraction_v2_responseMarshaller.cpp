#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_v2_response.h"
#include "Result_v2Marshaller.h"
#include "TriggerSourceMarshaller.h"

#include "PerformInteraction_v2_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool PerformInteraction_v2_responseMarshaller::checkIntegrity(PerformInteraction_v2_response& s)
{
  return checkIntegrityConst(s);
}


bool PerformInteraction_v2_responseMarshaller::fromString(const std::string& s,PerformInteraction_v2_response& e)
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


const std::string PerformInteraction_v2_responseMarshaller::toString(const PerformInteraction_v2_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PerformInteraction_v2_responseMarshaller::checkIntegrityConst(const PerformInteraction_v2_response& s)
{
  if(!Result_v2Marshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.info && s.info->length()>1000)  return false;
  if(s.choiceID && *s.choiceID>2000000000)  return false;
  if(s.triggerSource && !TriggerSourceMarshaller::checkIntegrityConst(*s.triggerSource))  return false;
  return true;
}

Json::Value PerformInteraction_v2_responseMarshaller::toJSON(const PerformInteraction_v2_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("PerformInteraction_v2");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=Result_v2Marshaller::toJSON(e.resultCode);

  if(e.info)
    j["info"]=Json::Value(*e.info);

  if(e.choiceID)
    j["choiceID"]=Json::Value(*e.choiceID);

  if(e.triggerSource)
    j["triggerSource"]=TriggerSourceMarshaller::toJSON(*e.triggerSource);

  json["response"]["parameters"]=j;
  return json;
}


bool PerformInteraction_v2_responseMarshaller::fromJSON(const Json::Value& js,PerformInteraction_v2_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.choiceID)  delete c.choiceID;
  c.choiceID=0;

  if(c.triggerSource)  delete c.triggerSource;
  c.triggerSource=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("response"))  return false;

    if(!js["response"].isObject())  return false;
    const Json::Value& j2=js["response"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("PerformInteraction_v2"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
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
      if(!Result_v2Marshaller::fromJSON(j,c.resultCode))
        return false;
    }
    if(json.isMember("info"))
    {
      const Json::Value& j=json["info"];
      if(!j.isString())  return false;
      c.info=new std::string(j.asString());
    }
    if(json.isMember("choiceID"))
    {
      const Json::Value& j=json["choiceID"];
      if(!j.isInt())  return false;
      c.choiceID=new unsigned int(j.asInt());
    }
    if(json.isMember("triggerSource"))
    {
      const Json::Value& j=json["triggerSource"];
      c.triggerSource=new TriggerSource();
      if(!TriggerSourceMarshaller::fromJSON(j,c.triggerSource[0]))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

