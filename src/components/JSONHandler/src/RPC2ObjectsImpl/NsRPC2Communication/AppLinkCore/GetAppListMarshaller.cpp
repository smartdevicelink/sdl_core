#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppList.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetAppListMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool GetAppListMarshaller::checkIntegrity(GetAppList& s)
{
  return checkIntegrityConst(s);
}


bool GetAppListMarshaller::fromString(const std::string& s,GetAppList& e)
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


const std::string GetAppListMarshaller::toString(const GetAppList& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetAppListMarshaller::checkIntegrityConst(const GetAppList& s)
{
  return true;
}


Json::Value GetAppListMarshaller::toJSON(const GetAppList& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.GetAppList");

  json["id"]=Json::Value(e.getId());
  return json;
}


bool GetAppListMarshaller::fromJSON(const Json::Value& json,GetAppList& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.GetAppList"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
