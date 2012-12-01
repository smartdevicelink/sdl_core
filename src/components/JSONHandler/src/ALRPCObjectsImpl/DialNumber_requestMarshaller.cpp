#include "../include/JSONHandler/ALRPCObjects/DialNumber_request.h"


#include "DialNumber_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool DialNumber_requestMarshaller::checkIntegrity(DialNumber_request& s)
{
  return checkIntegrityConst(s);
}


bool DialNumber_requestMarshaller::fromString(const std::string& s,DialNumber_request& e)
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


const std::string DialNumber_requestMarshaller::toString(const DialNumber_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DialNumber_requestMarshaller::checkIntegrityConst(const DialNumber_request& s)
{
  if(s.number.length()>500)  return false;
  return true;
}

Json::Value DialNumber_requestMarshaller::toJSON(const DialNumber_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("DialNumber");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["number"]=Json::Value(e.number);

  json["request"]["parameters"]=j;
  return json;
}


bool DialNumber_requestMarshaller::fromJSON(const Json::Value& js,DialNumber_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("DialNumber"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("number"))  return false;
    {
      const Json::Value& j=json["number"];
      if(!j.isString())  return false;
      c.number=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

