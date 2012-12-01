#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilities.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool GetCapabilitiesMarshaller::checkIntegrity(GetCapabilities& s)
{
  return checkIntegrityConst(s);
}


bool GetCapabilitiesMarshaller::fromString(const std::string& s,GetCapabilities& e)
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


const std::string GetCapabilitiesMarshaller::toString(const GetCapabilities& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetCapabilitiesMarshaller::checkIntegrityConst(const GetCapabilities& s)
{
  return true;
}


Json::Value GetCapabilitiesMarshaller::toJSON(const GetCapabilities& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.GetCapabilities");

  json["id"]=Json::Value(e.getId());
  return json;
}


bool GetCapabilitiesMarshaller::fromJSON(const Json::Value& json,GetCapabilities& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.GetCapabilities"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
