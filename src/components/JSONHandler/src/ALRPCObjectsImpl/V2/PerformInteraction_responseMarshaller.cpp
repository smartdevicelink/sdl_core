#include "../include/JSONHandler/ALRPCObjects/V2/PerformInteraction_response.h"
#include "ResultMarshaller.h"
#include "TriggerSourceMarshaller.h"

#include "PerformInteraction_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  if(e.choiceID)
    json["choiceID"]=Json::Value(*e.choiceID);

  if(e.triggerSource)
    json["triggerSource"]=TriggerSourceMarshaller::toJSON(*e.triggerSource);

  return json;
}


bool PerformInteraction_responseMarshaller::fromJSON(const Json::Value& json,PerformInteraction_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.choiceID)  delete c.choiceID;
  c.choiceID=0;

  if(c.triggerSource)  delete c.triggerSource;
  c.triggerSource=0;

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

