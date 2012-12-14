#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetDisplayLayout.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetDisplayLayoutMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool SetDisplayLayoutMarshaller::checkIntegrity(SetDisplayLayout& s)
{
  return checkIntegrityConst(s);
}


bool SetDisplayLayoutMarshaller::fromString(const std::string& s,SetDisplayLayout& e)
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


const std::string SetDisplayLayoutMarshaller::toString(const SetDisplayLayout& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetDisplayLayoutMarshaller::checkIntegrityConst(const SetDisplayLayout& s)
{
  return true;
}


Json::Value SetDisplayLayoutMarshaller::toJSON(const SetDisplayLayout& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.SetDisplayLayout");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["displayLayout"]=Json::Value(e.displayLayout);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool SetDisplayLayoutMarshaller::fromJSON(const Json::Value& json,SetDisplayLayout& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.SetDisplayLayout"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("displayLayout") || !js["displayLayout"].isString())  return false;
    c.displayLayout=js["displayLayout"].asString();
    
    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
