#include "../../include/JSONHandler/ALRPCObjects/UnsubscribeButton_request.h"
#include "ButtonNameMarshaller.h"

#include "UnsubscribeButton_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


bool UnsubscribeButton_requestMarshaller::checkIntegrity(UnsubscribeButton_request& s)
{
  return checkIntegrityConst(s);
}


bool UnsubscribeButton_requestMarshaller::fromString(const std::string& s,UnsubscribeButton_request& e)
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


const std::string UnsubscribeButton_requestMarshaller::toString(const UnsubscribeButton_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UnsubscribeButton_requestMarshaller::checkIntegrityConst(const UnsubscribeButton_request& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.buttonName))  return false;
  return true;
}

Json::Value UnsubscribeButton_requestMarshaller::toJSON(const UnsubscribeButton_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("UnsubscribeButton");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  json["request"]["parameters"]=j;
  return json;
}


bool UnsubscribeButton_requestMarshaller::fromJSON(const Json::Value& js,UnsubscribeButton_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("UnsubscribeButton"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonNameMarshaller::fromJSON(j,c.buttonName))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

