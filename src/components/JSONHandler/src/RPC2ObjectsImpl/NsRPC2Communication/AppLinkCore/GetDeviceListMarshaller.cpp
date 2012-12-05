#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetDeviceList.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetDeviceListMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool GetDeviceListMarshaller::checkIntegrity(GetDeviceList& s)
{
  return checkIntegrityConst(s);
}


bool GetDeviceListMarshaller::fromString(const std::string& s,GetDeviceList& e)
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


const std::string GetDeviceListMarshaller::toString(const GetDeviceList& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDeviceListMarshaller::checkIntegrityConst(const GetDeviceList& s)
{
  return true;
}


Json::Value GetDeviceListMarshaller::toJSON(const GetDeviceList& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.GetDeviceList");

  json["id"]=Json::Value(e.getId());
  return json;
}


bool GetDeviceListMarshaller::fromJSON(const Json::Value& json,GetDeviceList& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.GetDeviceList"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
