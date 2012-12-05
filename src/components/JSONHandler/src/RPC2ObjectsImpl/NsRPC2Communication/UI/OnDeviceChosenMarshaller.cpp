#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDeviceChosen.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnDeviceChosenMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool OnDeviceChosenMarshaller::checkIntegrity(OnDeviceChosen& s)
{
  return checkIntegrityConst(s);
}


bool OnDeviceChosenMarshaller::fromString(const std::string& s,OnDeviceChosen& e)
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


const std::string OnDeviceChosenMarshaller::toString(const OnDeviceChosen& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnDeviceChosenMarshaller::checkIntegrityConst(const OnDeviceChosen& s)
{
  return true;
}


Json::Value OnDeviceChosenMarshaller::toJSON(const OnDeviceChosen& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnDeviceChosen");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["deviceName"]=Json::Value(e.deviceName);;
  return json;
}


bool OnDeviceChosenMarshaller::fromJSON(const Json::Value& json,OnDeviceChosen& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnDeviceChosen"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("deviceName") || !js["deviceName"].isString())  return false;
    c.deviceName=js["deviceName"].asString();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
