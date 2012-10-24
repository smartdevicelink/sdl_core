#include "../../include/JSONHandler/ALRPCObjects/GetDTCs_request.h"


#include "GetDTCs_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool GetDTCs_requestMarshaller::checkIntegrity(GetDTCs_request& s)
{
  return checkIntegrityConst(s);
}


bool GetDTCs_requestMarshaller::fromString(const std::string& s,GetDTCs_request& e)
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


const std::string GetDTCs_requestMarshaller::toString(const GetDTCs_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDTCs_requestMarshaller::checkIntegrityConst(const GetDTCs_request& s)
{
  if(s.ecuName && *s.ecuName>65535)  return false;
  return true;
}

Json::Value GetDTCs_requestMarshaller::toJSON(const GetDTCs_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("GetDTCs");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.ecuName)
    j["ecuName"]=Json::Value(*e.ecuName);

  if(e.encrypted)
    j["encrypted"]=Json::Value(*e.encrypted);

  json["request"]["parameters"]=j;
  return json;
}


bool GetDTCs_requestMarshaller::fromJSON(const Json::Value& js,GetDTCs_request& c)
{
  if(c.ecuName)  delete c.ecuName;
  c.ecuName=0;

  if(c.encrypted)  delete c.encrypted;
  c.encrypted=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("GetDTCs"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("ecuName"))
    {
      const Json::Value& j=json["ecuName"];
      if(!j.isInt())  return false;
      c.ecuName=new unsigned int(j.asInt());
    }
    if(json.isMember("encrypted"))
    {
      const Json::Value& j=json["encrypted"];
      if(!j.isBool())  return false;
      c.encrypted=new bool(j.asBool());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

