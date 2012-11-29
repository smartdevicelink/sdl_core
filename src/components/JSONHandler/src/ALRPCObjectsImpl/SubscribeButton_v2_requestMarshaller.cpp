#include "../include/JSONHandler/ALRPCObjects/SubscribeButton_v2_request.h"
#include "ButtonName_v2Marshaller.h"

#include "SubscribeButton_v2_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool SubscribeButton_v2_requestMarshaller::checkIntegrity(SubscribeButton_v2_request& s)
{
  return checkIntegrityConst(s);
}


bool SubscribeButton_v2_requestMarshaller::fromString(const std::string& s,SubscribeButton_v2_request& e)
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


const std::string SubscribeButton_v2_requestMarshaller::toString(const SubscribeButton_v2_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SubscribeButton_v2_requestMarshaller::checkIntegrityConst(const SubscribeButton_v2_request& s)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(s.buttonName))  return false;
  return true;
}

Json::Value SubscribeButton_v2_requestMarshaller::toJSON(const SubscribeButton_v2_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("SubscribeButton_v2");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["buttonName"]=ButtonName_v2Marshaller::toJSON(e.buttonName);

  json["request"]["parameters"]=j;
  return json;
}


bool SubscribeButton_v2_requestMarshaller::fromJSON(const Json::Value& js,SubscribeButton_v2_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("SubscribeButton_v2"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonName_v2Marshaller::fromJSON(j,c.buttonName))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

