#include "../../include/JSONHandler/ALRPCObjects/ListFiles_request.h"


#include "ListFiles_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool ListFiles_requestMarshaller::checkIntegrity(ListFiles_request& s)
{
  return checkIntegrityConst(s);
}


bool ListFiles_requestMarshaller::fromString(const std::string& s,ListFiles_request& e)
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


const std::string ListFiles_requestMarshaller::toString(const ListFiles_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ListFiles_requestMarshaller::checkIntegrityConst(const ListFiles_request& s)
{
  return true;
}

Json::Value ListFiles_requestMarshaller::toJSON(const ListFiles_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("ListFiles");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  json["request"]["parameters"]=j;
  return json;
}


bool ListFiles_requestMarshaller::fromJSON(const Json::Value& js,ListFiles_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ListFiles"))  return false;
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

