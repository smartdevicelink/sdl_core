#include "../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_request.h"


#include "UnregisterAppInterface_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;


bool UnregisterAppInterface_requestMarshaller::checkIntegrity(UnregisterAppInterface_request& s)
{
  return checkIntegrityConst(s);
}


bool UnregisterAppInterface_requestMarshaller::fromString(const std::string& s,UnregisterAppInterface_request& e)
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


const std::string UnregisterAppInterface_requestMarshaller::toString(const UnregisterAppInterface_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UnregisterAppInterface_requestMarshaller::checkIntegrityConst(const UnregisterAppInterface_request& s)
{
  return true;
}

Json::Value UnregisterAppInterface_requestMarshaller::toJSON(const UnregisterAppInterface_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("UnregisterAppInterface");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  json["request"]["parameters"]=j;
  return json;
}


bool UnregisterAppInterface_requestMarshaller::fromJSON(const Json::Value& js,UnregisterAppInterface_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("UnregisterAppInterface"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

