#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddCommand.h"
#include "../src/ALRPCObjectsImpl/MenuParamsMarshaller.h"
#include "../src/ALRPCObjectsImpl/ImageMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddCommandMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool AddCommandMarshaller::checkIntegrity(AddCommand& s)
{
  return checkIntegrityConst(s);
}


bool AddCommandMarshaller::fromString(const std::string& s,AddCommand& e)
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


const std::string AddCommandMarshaller::toString(const AddCommand& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddCommandMarshaller::checkIntegrityConst(const AddCommand& s)
{
  if(s.cmdId>2000000000)  return false;

  if(!NsAppLinkRPC::MenuParamsMarshaller::checkIntegrityConst(s.menuParams))  return false;

  if(s.cmdIcon && (!NsAppLinkRPC::ImageMarshaller::checkIntegrityConst(s.cmdIcon[0])))  return false;

  return true;
}


Json::Value AddCommandMarshaller::toJSON(const AddCommand& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.AddCommand");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["cmdId"]=Json::Value(e.cmdId);;
  json["params"]["menuParams"]=NsAppLinkRPC::MenuParamsMarshaller::toJSON(e.menuParams);;
  if(e.cmdIcon)
    json["params"]["cmdIcon"]=NsAppLinkRPC::ImageMarshaller::toJSON(e.cmdIcon[0]);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool AddCommandMarshaller::fromJSON(const Json::Value& json,AddCommand& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.AddCommand"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("cmdId") || !js["cmdId"].isInt())  return false;
    c.cmdId=js["cmdId"].asInt();
    if(c.cmdId>2000000000)  return false;

    if(!js.isMember("menuParams") || !NsAppLinkRPC::MenuParamsMarshaller::fromJSON(js["menuParams"],c.menuParams))  return false;

    if(c.cmdIcon)  delete c.cmdIcon;
    c.cmdIcon=0;
    if(js.isMember("cmdIcon"))
    {
      c.cmdIcon=new NsAppLinkRPC::Image();
      if(!NsAppLinkRPC::ImageMarshaller::fromJSON(js["cmdIcon"],c.cmdIcon[0]))  return false;
    }

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
