#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetAppIcon.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetAppIconMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool SetAppIconMarshaller::checkIntegrity(SetAppIcon& s)
{
  return checkIntegrityConst(s);
}


bool SetAppIconMarshaller::fromString(const std::string& s,SetAppIcon& e)
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


const std::string SetAppIconMarshaller::toString(const SetAppIcon& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetAppIconMarshaller::checkIntegrityConst(const SetAppIcon& s)
{
  if(s.syncFileName.length()>500)  return false;

  return true;
}


Json::Value SetAppIconMarshaller::toJSON(const SetAppIcon& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.SetAppIcon");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["syncFileName"]=Json::Value(e.syncFileName);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool SetAppIconMarshaller::fromJSON(const Json::Value& json,SetAppIcon& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.SetAppIcon"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("syncFileName") || !js["syncFileName"].isString())  return false;
    c.syncFileName=js["syncFileName"].asString();
    if(c.syncFileName.length()>500)  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
