#include "../include/JSONHandler/ALRPCObjects/ScrollableMessage_response.h"
#include "Result_v2Marshaller.h"

#include "ScrollableMessage_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool ScrollableMessage_responseMarshaller::checkIntegrity(ScrollableMessage_response& s)
{
  return checkIntegrityConst(s);
}


bool ScrollableMessage_responseMarshaller::fromString(const std::string& s,ScrollableMessage_response& e)
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


const std::string ScrollableMessage_responseMarshaller::toString(const ScrollableMessage_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ScrollableMessage_responseMarshaller::checkIntegrityConst(const ScrollableMessage_response& s)
{
  if(!Result_v2Marshaller::checkIntegrityConst(s.resultCode))  return false;
  return true;
}

Json::Value ScrollableMessage_responseMarshaller::toJSON(const ScrollableMessage_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("ScrollableMessage");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=Result_v2Marshaller::toJSON(e.resultCode);

  json["response"]["parameters"]=j;
  return json;
}


bool ScrollableMessage_responseMarshaller::fromJSON(const Json::Value& js,ScrollableMessage_response& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("response"))  return false;

    if(!js["response"].isObject())  return false;
    const Json::Value& j2=js["response"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ScrollableMessage"))  return false;
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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

