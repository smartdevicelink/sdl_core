#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleData.h"
#include "../src/ALRPCObjectsImpl/V2/VehicleDataTypeMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleDataMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool GetVehicleDataMarshaller::checkIntegrity(GetVehicleData& s)
{
  return checkIntegrityConst(s);
}


bool GetVehicleDataMarshaller::fromString(const std::string& s,GetVehicleData& e)
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


const std::string GetVehicleDataMarshaller::toString(const GetVehicleData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetVehicleDataMarshaller::checkIntegrityConst(const GetVehicleData& s)
{
  if(!NsAppLinkRPCV2::VehicleDataTypeMarshaller::checkIntegrityConst(s.dataType))  return false;

  return true;
}


Json::Value GetVehicleDataMarshaller::toJSON(const GetVehicleData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("VehicleInfo.GetVehicleData");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["dataType"]=NsAppLinkRPCV2::VehicleDataTypeMarshaller::toJSON(e.dataType);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool GetVehicleDataMarshaller::fromJSON(const Json::Value& json,GetVehicleData& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("VehicleInfo.GetVehicleData"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("dataType") || !NsAppLinkRPCV2::VehicleDataTypeMarshaller::fromJSON(js["dataType"],c.dataType))  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
