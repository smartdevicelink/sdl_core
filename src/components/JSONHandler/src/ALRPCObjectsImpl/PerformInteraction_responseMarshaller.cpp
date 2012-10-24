#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"
#include "ResultMarshaller.h"
#include "TriggerSourceMarshaller.h"

#include "PerformInteraction_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool PerformInteraction_responseMarshaller::checkIntegrity(PerformInteraction_response& s)
{
  return checkIntegrityConst(s);
}


bool PerformInteraction_responseMarshaller::fromString(const std::string& s,PerformInteraction_response& e)
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


const std::string PerformInteraction_responseMarshaller::toString(const PerformInteraction_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PerformInteraction_responseMarshaller::checkIntegrityConst(const PerformInteraction_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.info && s.info->length()>1000)  return false;
  if(s.choiceID && *s.choiceID>2000000000)  return false;
  if(s.triggerSource && !TriggerSourceMarshaller::checkIntegrityConst(*s.triggerSource))  return false;
  return true;
}

Json::Value PerformInteraction_responseMarshaller::toJSON(const PerformInteraction_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("PerformInteraction");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    j["info"]=Json::Value(*e.info);

  if(e.choiceID)
    j["choiceID"]=Json::Value(*e.choiceID);

  if(e.triggerSource)
    j["triggerSource"]=TriggerSourceMarshaller::toJSON(*e.triggerSource);

  json["response"]["parameters"]=j;
  return json;
}


bool PerformInteraction_responseMarshaller::fromJSON(const Json::Value& js,PerformInteraction_response& c)
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

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("PerformInteraction"))  return false;
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
      if(!ResultMarshaller::fromJSON(j,c.resultCode))
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

