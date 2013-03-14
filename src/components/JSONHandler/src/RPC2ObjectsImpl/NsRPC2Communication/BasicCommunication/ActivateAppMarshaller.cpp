#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/ActivateApp.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/RPC2ObjectsImpl/NsRPC2Communication/BasicCommunication/ActivateAppMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::BasicCommunication;

bool ActivateAppMarshaller::checkIntegrity(ActivateApp& s)
{
  return checkIntegrityConst(s);
}


bool ActivateAppMarshaller::fromString(const std::string& s,ActivateApp& e)
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


const std::string ActivateAppMarshaller::toString(const ActivateApp& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ActivateAppMarshaller::checkIntegrityConst(const ActivateApp& s)
{
  if(s.appName.length()>100)  return false;

  return true;
}


Json::Value ActivateAppMarshaller::toJSON(const ActivateApp& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("BasicCommunication.ActivateApp");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["appName"]=Json::Value(e.appName);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ActivateAppMarshaller::fromJSON(const Json::Value& json,ActivateApp& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("BasicCommunication.ActivateApp"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("appName") || !js["appName"].isString())  return false;
    c.appName=js["appName"].asString();
    if(c.appName.length()>100)  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
