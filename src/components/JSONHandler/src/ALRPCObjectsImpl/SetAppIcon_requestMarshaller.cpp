#include "../include/JSONHandler/ALRPCObjects/SetAppIcon_request.h"


#include "SetAppIcon_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool SetAppIcon_requestMarshaller::checkIntegrity(SetAppIcon_request& s)
{
  return checkIntegrityConst(s);
}


bool SetAppIcon_requestMarshaller::fromString(const std::string& s,SetAppIcon_request& e)
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


const std::string SetAppIcon_requestMarshaller::toString(const SetAppIcon_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetAppIcon_requestMarshaller::checkIntegrityConst(const SetAppIcon_request& s)
{
  if(s.syncFileName && s.syncFileName->length()>500)  return false;
  return true;
}

Json::Value SetAppIcon_requestMarshaller::toJSON(const SetAppIcon_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("SetAppIcon");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.syncFileName)
    j["syncFileName"]=Json::Value(*e.syncFileName);

  json["request"]["parameters"]=j;
  return json;
}


bool SetAppIcon_requestMarshaller::fromJSON(const Json::Value& js,SetAppIcon_request& c)
{
  if(c.syncFileName)  delete c.syncFileName;
  c.syncFileName=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("SetAppIcon"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("syncFileName"))
    {
      const Json::Value& j=json["syncFileName"];
      if(!j.isString())  return false;
      c.syncFileName=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

